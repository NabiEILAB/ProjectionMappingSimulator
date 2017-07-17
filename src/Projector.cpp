//
//  Projector.cpp
//  ProjectorSimulator
//
//  Created by Nabi Lab on 2017. 7. 17..
//
//

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
    projectorParameters.add(xRotation.set("X Rotate",xRotation,0,360));
    projectorParameters.add(yRotation.set("Y Rotate",yRotation,0,360));
    projectorParameters.add(zRotation.set("Z Rotate",zRotation,0,360));
    
    //videoPlayer.load("*.*");
    //videoPlayer.play();
}

void Projector::update() {
    //videoPlayer.update();
}

void Projector::draw() {
    ofPushMatrix();
    ofPushStyle();
    
    ofSetColor(255,255,255);
    ofDrawBitmapString("No." + ofToString(projectorNum) + " Projector (" + ofToString(xPos) + ", " + ofToString(yPos) + ", " + ofToString(zPos) + ")", xPos, yPos + 20, zPos);
    
    ofTranslate(xPos,yPos,zPos);
    ofRotate(xRotation,1,0,0);
    ofRotate(yRotation,0,1,0);
    ofRotate(zRotation,0,0,1);
    ofTranslate(-xPos,-yPos,-zPos);
    
    ofSetColor(100 + 37, 100 + 12, 100 + 68); //Projector color
    ofDrawBox(xPos,yPos,zPos,20,5,10);
    
    //Draw light radiation line for about 50 pixels ahead
    ofSetColor(255,0,0);
    ofDrawLine(xPos,yPos,zPos, xPos - xRadVal * 50, yPos + yRadVal * 50, zPos - zRadVal * 50);
    ofDrawLine(xPos,yPos,zPos, xPos + xRadVal * 50, yPos + yRadVal * 50, zPos - zRadVal * 50);
    ofDrawLine(xPos,yPos,zPos, xPos - xRadVal * 50, yPos - yRadVal * 50, zPos - zRadVal * 50);
    ofDrawLine(xPos,yPos,zPos, xPos + xRadVal * 50, yPos - yRadVal * 50, zPos - zRadVal * 50);
    
    ofPopStyle();
    ofPopMatrix();
}
