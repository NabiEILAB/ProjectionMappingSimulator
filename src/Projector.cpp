//
//  Projector.cpp
//  ProjectorSimulator
//
//  Created by Nabi Lab on 2017. 7. 17..
//
//

#define DEG2RAD 3.141592f/180.0;

#include "Projector.h"

Projector::Projector() {
    this->xPos = 0;
    this->yPos = 0;
    this->zPos = 0;
    this->xRotation = 0;
    this->yRotation = 0;
    this->zRotation = 0;
    isSelected = false;
    isTrsMode = false;
}

Projector::Projector(float xPos, float yPos, float zPos) {
    this->xPos = xPos;
    this->yPos = yPos;
    this->zPos = zPos;
    this->xRotation = 0;
    this->yRotation = 0;
    this->zRotation = 0;
    isSelected = false;
    isTrsMode = false;
}

Projector::Projector(float xPos, float yPos, float zPos, float xRotation, float yRotation, float zRotation) {
    this->xPos = xPos;
    this->yPos = yPos;
    this->zPos = zPos;
    this->xRotation = xRotation;
    this->yRotation = yRotation;
    this->zRotation = zRotation;
    isSelected = false;
    isTrsMode = false;
}

Projector::Projector(float xPos, float yPos, float zPos, float xRotation, float yRotation, float zRotation, int projectorNum) {
    this->xPos = xPos;
    this->yPos = yPos;
    this->zPos = zPos;
    this->xRotation = xRotation;
    this->yRotation = yRotation;
    this->zRotation = zRotation;
    this->projectorNum = projectorNum = projectorNum;
    isSelected = false;
    isTrsMode = false;
}

void Projector::setup() {
    projectorParameters.setName("No." + ofToString(projectorNum) + " Projector");
    //projectorParameters.add(isPlaying.set("Video Play",false));
    projectorParameters.add(xPos.set("X Pos",xPos,-1000,1000));
    projectorParameters.add(yPos.set("Y Pos",yPos,-1000,1000));
    projectorParameters.add(zPos.set("Z Pos",zPos,-1000,1000));
    projectorParameters.add(xRotation.set("X Rotate",xRotation,-360,360));
    projectorParameters.add(yRotation.set("Y Rotate",yRotation,-360,360));
    projectorParameters.add(zRotation.set("Z Rotate",zRotation,-360,360));
    
    videoPlayer.load("*.*");
    videoPlayer.play();
    
    //videoImg.allocate(videoPlayer.getWidth(), videoPlayer.getHeight(), ofImageType::OF_IMAGE_COLOR_ALPHA);
    colorImg.allocate(videoPlayer.getWidth(), videoPlayer.getHeight());
}

void Projector::update() {
    videoPlayer.update();
    //videoImg.setFromPixels(videoPlayer.getPixels());
    colorImg.setFromPixels(videoPlayer.getPixels());
}

void Projector::draw() {
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor(255,255,255);
    //ofDrawBitmapString("No." + ofToString(projectorNum) + " Projector (" + ofToString(xPos) + ", " + ofToString(yPos) + ", " + ofToString(zPos) + ")", xPos, yPos + 20, zPos);
    
    ofTranslate(xPos,yPos,zPos);
    ofRotate(xRotation,1,0,0);
    ofRotate(yRotation,0,1,0);
    ofRotate(zRotation,0,0,1);
    ofTranslate(-xPos,-yPos,-zPos);
    
    //Projector color
    //ofSetColor(100 + 37, 100 + 12, 100 + 68);
    //ofDrawBox(xPos,yPos,zPos,20,5,10);
    
    //Draw light radiation line for about 50 pixels ahead
    ofSetColor(255,0,0);
    ofDrawLine(xPos,yPos,zPos, xPos - xRadVal * 50, yPos + yRadVal * 50, zPos - zRadVal * 50);
    ofDrawLine(xPos,yPos,zPos, xPos + xRadVal * 50, yPos + yRadVal * 50, zPos - zRadVal * 50);
    ofDrawLine(xPos,yPos,zPos, xPos - xRadVal * 50, yPos - yRadVal * 50, zPos - zRadVal * 50);
    ofDrawLine(xPos,yPos,zPos, xPos + xRadVal * 50, yPos - yRadVal * 50, zPos - zRadVal * 50);
    
    //Depth limit. if vertex's z value is far more than left operand('-500' in this case) then, texturing won't come up
    //float distance = abs(0-zPos);
    float distance = abs(-500 - zPos); //calculate between -500 and zPos.
    
    ofVec3f topLeft = ofVec3f(xPos - (xRadVal * distance), yPos + (yRadVal * distance), zPos - (zRadVal * distance));
    ofVec3f topRight = ofVec3f(xPos + (xRadVal * distance), yPos + (yRadVal * distance), zPos - (zRadVal * distance));
    ofVec3f bottomLeft = ofVec3f(xPos - (xRadVal * distance), yPos - (yRadVal * distance), zPos - (zRadVal * distance));
    ofVec3f bottomRight = ofVec3f(xPos + (xRadVal * distance), yPos - (yRadVal * distance), zPos - (zRadVal * distance));
    float width = abs(topLeft.distance(topRight));
    float height = abs(topLeft.distance(bottomLeft));
    
    //Use texture 1. directly use itself -> can't resize the image in realtime
    //videoTexture = videoPlayer.getTexture();
    
    
    //Use texture 2. resize through ofImage -> rendering is very slow
    //videoImg.resize(width, height);
    
    
    //Use texture 3. resize through ofxCvColorImg
    colorImg.resize(width, height);
    videoTexture.allocate(colorImg.getPixels());
    videoTexture.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
    videoTexture.setTextureWrap(GL_CLAMP_TO_BORDER_ARB, GL_CLAMP_TO_BORDER_ARB);
    colorImg.clear();
    
    //Make matrices for Projective Texturing
    //Projector's view matrix
    ofVec3f projectorPos = ofVec3f(xPos, yPos, zPos);
    float sx, sy, sz, cx, cy, cz, theta;
    theta = xRotation * DEG2RAD;
    sx = sinf(theta);
    cx = cosf(theta);
    
    theta = yRotation * DEG2RAD;
    sy = sinf(theta);
    cy = cosf(theta);
    
    theta = zRotation * DEG2RAD;
    sz = sinf(theta);
    cz = cosf(theta);
    
    ofVec3f projectorTarget = ofVec3f(xPos - (sy * cz + cy * sx * sz), yPos - (sy * sz - cy * sx * cz), zPos - (cy * cx));
    ofVec3f projectorUp = ofVec3f(-1 * cx * sz, cx * cz, sx);
    //ofVec3f projectorUp = ofVec3f(0,1,0);
    
    projectorView.makeLookAtViewMatrix(projectorPos, projectorTarget, projectorUp);
    
    //Projector's projection matrix
    float aspect = float(width/height);
    projectorProjection = ofMatrix4x4::newPerspectiveMatrix(75, aspect, distance/1000, distance); //need to find a way to calculate proper fov value
    
    //0-1 bias matrix
    projectorBias = ofMatrix4x4::newIdentityMatrix();
    projectorBias.scale(ofVec3f(0.5, -0.5, 0.5)); //coordinate origin starts inversed.
    projectorBias.translate(ofVec3f(0.5, 0.5, 0.5));
    projectorBias.scale(ofVec3f(width, height, 1));
    
    //Projector matrix
    projectorMatrix = projectorView * projectorProjection * projectorBias;
    
    ofPopStyle();
    ofPopMatrix();
}
