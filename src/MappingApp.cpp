//
//  MappingApp.cpp
//  ProjectorSimulator
//
//  Created by Nabi E.I.Lab on 2017. 10. 12..
//
//

#include "MappingApp.h"

void MappingApp::setup() {
    ofBackground(128);
    projector = NULL;
    
    centerPt.x = 375;
    centerPt.y = 375;
}

void MappingApp::update() {
    if(projector == NULL)
        return ;
    
    if(projector->videoPlayer.isLoaded())
        texture.allocate(projector->videoPlayer.getPixels());
    
    realCenterPt.x = projector->width/2;
    realCenterPt.y = projector->height/2;
    toRealValue();
}

void MappingApp::draw() {
    if(projector == NULL)
        return ;
    
    string text = "Current selected Projector : ";
    text += ofToString(projector->projectorNum);
    
    if(projector->videoPlayer.isLoaded())
        texture.draw(points[0],points[1],points[2],points[3]);
    
    ofPushStyle();
    ofSetColor(64, 255, 64);
    ofDrawLine(leftX, upY, rightX, upY);
    ofDrawLine(rightX, upY, rightX, downY);
    ofDrawLine(rightX, downY, leftX, downY);
    ofDrawLine(leftX, downY, leftX, upY);
    
    ofNoFill();
    ofSetColor(255, 64, 64);
    ofDrawCircle(points[0], 5);
    ofDrawCircle(points[1], 5);
    ofDrawCircle(points[2], 5);
    ofDrawCircle(points[3], 5);
    ofDrawCircle(centerPt,5);
    
    ofSetColor(64,64,255);
    ofDrawLine(points[0], centerPt);
    ofDrawLine(points[1], centerPt);
    ofDrawLine(points[2], centerPt);
    ofDrawLine(points[3], centerPt);
    ofPopStyle();

    
    ofDrawBitmapString(text, 10, 10);
}

//--------------------------------------------------------------
void MappingApp::keyPressed(int key){
    switch(key) {
        case 'r' : {
            points[0].x = leftX; points[0].y = upY;
            points[1].x = rightX; points[1].y = upY;
            points[2].x = rightX; points[2].y = downY;
            points[3].x = leftX; points[3].y = downY;
            break;
        }
        default : {
            break;
        }
    }
}

//--------------------------------------------------------------
void MappingApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void MappingApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void MappingApp::mouseDragged(int x, int y, int button){
    float nearestDistance = 50;
    int nearestIndex = -1;
    for(int i=0; i<4; i++) {
        float distance = ofVec2f(points[i].x,points[i].y).distance(ofVec2f(x,y));
        if(distance < 50 && distance < nearestDistance) {
            nearestDistance = distance;
            nearestIndex = i;
        }
    }
    if(x >= leftX && x<= rightX && y >= upY && y <= downY) {
        points[nearestIndex].x = x;
        points[nearestIndex].y = y;
    }
    toRealValue();
}

//--------------------------------------------------------------
void MappingApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void MappingApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void MappingApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void MappingApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void MappingApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void MappingApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void MappingApp::dragEvent(ofDragInfo dragInfo){
    
}

void MappingApp::setProjector(Projector* proj) {
    projector = proj;
    int videoWidth = projector->videoPlayer.getWidth();
    int videoHeight = projector->videoPlayer.getHeight();
    
    realCenterPt.x = projector->width/2;
    realCenterPt.y = projector->height/2;
    
    int longerOne,shorterOne;
    float ratio;
    if(videoWidth > videoHeight) {
        longerOne = videoWidth;
        shorterOne = videoHeight;
        
        ratio = (float)shorterOne/longerOne;
        
        longerOne = 600;
        shorterOne = 600 * ratio;
        
        videoWidth = longerOne;
        videoHeight = shorterOne;
    }
    else {
        longerOne = videoHeight;
        shorterOne = videoWidth;
        
        ratio = (float)shorterOne/longerOne;
        
        longerOne = 600;
        shorterOne = 600 * ratio;
        
        videoWidth = shorterOne;
        videoHeight = longerOne;
    }
    
    points[0].x = centerPt.x - videoWidth/2;
    points[0].y = centerPt.y - videoHeight/2;
    
    points[1].x = centerPt.x + videoWidth/2;
    points[1].y = centerPt.y - videoHeight/2;
    
    points[2].x = centerPt.x + videoWidth/2;
    points[2].y = centerPt.y + videoHeight/2;
    
    points[3].x = centerPt.x - videoWidth/2;
    points[3].y = centerPt.y + videoHeight/2;
    
    leftX = points[0].x;
    rightX = points[1].x;
    upY = points[0].y;
    downY = points[2].y;
    
    if(!projector->isMappingOn) {
        projector->pt[0] = ofVec2f(0, projector->width);
        projector->pt[1] = ofVec2f(projector->width, 0);
        projector->pt[2] = ofVec2f(projector->width, projector->height);
        projector->pt[3] = ofVec2f(0, projector->height);
        
        toRealValue();
        
        projector->isMappingOn = true;
    }
    else {
        toUnrealValue();
    }
}

void MappingApp::setWarpingMode() {
    
}

void MappingApp::toRealValue() {
    float widthRatio = projector->width / (rightX - leftX);
    float heightRatio = projector->height / (downY - upY);
    
    for(int i=3; i>=0; i--) {
        float pointX = points[i].x - centerPt.x;
        float pointY = points[i].y - centerPt.y;
        
        int realIndex = 3-i;
        
        if(pointX > 0)
            realPoints[realIndex].x = realCenterPt.x + abs(pointX) * widthRatio;
        else
            realPoints[realIndex].x = realCenterPt.x - abs(pointX) * widthRatio;
                
        if(pointY > 0)
            realPoints[realIndex].y = realCenterPt.y - abs(pointY) * heightRatio;
        else
            realPoints[realIndex].y = realCenterPt.y + abs(pointY) * heightRatio;
        
        projector->pt[realIndex] = realPoints[realIndex];
    }
}

void MappingApp::toUnrealValue() {
    float widthRatio = (rightX - leftX) / projector->width;
    float heightRatio = (downY - upY) / projector->height;
    
    for(int i=3; i>=0; i--) {
        float pointX = projector->pt[3-i].x - realCenterPt.x;
        float pointY = projector->pt[3-i].y - realCenterPt.y;
        
        if(pointX > 0)
            points[i].x = centerPt.x + abs(pointX) * widthRatio;
        else
            points[i].x = centerPt.x - abs(pointX) * widthRatio;
        
        if(pointY > 0)
            points[i].y = centerPt.y - abs(pointY) * heightRatio;
        else
            points[i].y = centerPt.y + abs(pointY) * heightRatio;
    }
}

