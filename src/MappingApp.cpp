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
    
    panelWindow.load("UI/Panel/panelWindow.png");
    panelGrayBar.load("UI/Panel/grayBar.png");
    panelGreenBar.load("UI/Panel/greenBar.png");
    panelSliderButton.load("UI/Panel/sliderButton.png");
    panelClickIndex = -1;
    ofEnableDepthTest();
    
    manufacturerMenu.load("UI/DropDown/chooseMenu.png");
    manufacturerClick.load("UI/DropDown/chooseMenuClick.png");
    modelMenu.load("UI/DropDown/chooseMenu.png");
    modelClick.load("UI/DropDown/chooseMenuClick.png");
    manuHoverIndex = -1;
    modelHoverIndex = -1;
    manuClicked = false;
    modelClicked = false;
    
    topMenu.load("UI/DropDown/topMenu.png");
    topMenuHover.load("UI/DropDown/topMenuHover.png");
    centerMenu.load("UI/DropDown/centerMenu.png");
    centerMenuHover.load("UI/DropDown/centerMenuHover.png");
    bottomMenu.load("UI/DropDown/bottomMenu.png");
    bottomMenuHover.load("UI/DropDown/bottomMenuHover.png");
    check.load("UI/DropDown/checkIcon.png");
    
    string str;
    ofFile file;
    ofBuffer buffer;
    file.open("Spec/projector_spec_list.txt");
    buffer = file.readToBuffer();
    for(auto line : buffer.getLines())
        specList.push_back(line);
    
    for(int i=0; i<specList.size(); i++) {
        if(i==0)
            manufacturerNameList.push_back(ofSplitString(specList[i],",")[0]);
        else {
            if(ofSplitString(specList[i],",")[0].compare(ofSplitString(specList[i-1],",")[0])!=0)
               manufacturerNameList.push_back(ofSplitString(specList[i],",")[0]);
        }
        /*if(i!=0 && ofSplitString(specList[i],",")[0].compare("Barco")==0) {
            ofLog() << ofSplitString(specList[i],",")[1];
            ofLog() << ofSplitString(specList[i],",")[2];
            ofLog() << ofSplitString(specList[i],",")[3];
            ofLog() << ofSplitString(specList[i],",")[4];
        }*/
    }
    
    /*for(int i=0; i<manufacturerNameList.size(); i++)
        ofLog() << manufacturerNameList[i];*/
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
    
    if(projector->videoPlayer.isLoaded()) {
        texture.draw(points[0],points[1],points[2],points[3]);
        
        ofDrawBitmapString("most far distance : " + ofToString(abs(0 - projector->zPos)), 10, 30);
        ofDrawBitmapString("width : " + ofToString(projector->width), 10, 40);
        ofDrawBitmapString("height : " + ofToString(projector->height), 10, 50);
        
        /*ofDisableDepthTest();
        drawPanel();
        ofEnableDepthTest();*/
        
        /*ofPushStyle();
        ofNoFill();
        ofSetColor(0, 255, 0);
        ofDrawCurve(leftX, upY + 50, leftX, upY, rightX/2, upY + 150, rightX, upY + 50);
        ofDrawCurve(leftX + 300, upY, leftX, upY, leftX, downY, leftX + 300, downY);
        ofPopStyle();*/
    }
    
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
    
    ofDisableDepthTest();
    drawChooseMenu();
    ofEnableDepthTest();
    
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
    if(manuClicked) {
        float currentYPos = 30 + manufacturerMenu.getHeight();
        if(x > 10 && x <= 10 + manufacturerMenu.getWidth() * 1.5) {
            for(int i = 0; i < manufacturerNameList.size(); i++) {
                manuHoverIndex = -1;
                if(y > currentYPos && y <= currentYPos + manufacturerMenu.getHeight()) {
                    manuHoverIndex = i;
                    break;
                }
                currentYPos += manufacturerMenu.getHeight();
            }
        }
        else
            manuHoverIndex = -1;
    }
    else if(modelClicked) {
        float currentYPos = 90 + modelMenu.getHeight();
        if(x > 10 && x <= 10 + modelMenu.getWidth()) {
            for(int i = 0; i < modelNameList.size(); i++) {
                modelHoverIndex = -1;
                if(y > currentYPos && y <= currentYPos + modelMenu.getHeight()) {
                    modelHoverIndex = i;
                    break;
                }
                currentYPos += modelMenu.getHeight();
            }
        }
        else
            modelHoverIndex = -1;
    }
}

//--------------------------------------------------------------
void MappingApp::mouseDragged(int x, int y, int button){
    if(panelClickIndex != -1) {
        if(x >= 135 && x <= 135 + 250) {
            if(panelClickIndex==0) {
                float realValue = (float)(x - 135) / 250 * 3000;
                projector->pivotDistance = realValue;
                //setProjector(projector);
            }
            else if(panelClickIndex==1) {
                float realValue = (float)(x - 135) / 250 * 5000;
                projector->pivotWidth = realValue;
                //setProjector(projector);
                
            }
            else if(panelClickIndex==2) {
                float realValue = (float)(x - 135) / 250 * 5000;
                projector->pivotHeight = realValue;
                //setProjector(projector);
            }
        }
    }
    else {
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
}

//--------------------------------------------------------------
void MappingApp::mousePressed(int x, int y, int button){
    /*if(projector->videoPlayer.isLoaded() && panelClickIndex == -1) {
        float videoPivotDistance = projector->pivotDistance;
        float videoPivotWidth = projector->pivotWidth;
        float videoPivotHeight = projector->pivotHeight;
        if(y > ofGetHeight() - 110 && y <= ofGetHeight() - 100) {
            float lengthRatio = videoPivotDistance / 3000;
            float barLength = 250 * lengthRatio;
            if(x > 135 + barLength - 10 && x <= 135 + barLength + 10) {
                panelClickIndex = 0;
                //setProjector(projector);
            }
        }
        else if(y > ofGetHeight() - 80 && y <= ofGetHeight() - 70) {
            float lengthRatio = videoPivotWidth / 5000;
            float barLength = 250 * lengthRatio;
            if(x > 135 + barLength - 10 && x <= 135 + barLength + 10) {
                panelClickIndex = 1;
                //setProjector(projector);
            }
        }
        else if(y > ofGetHeight() - 50 && y <= ofGetHeight() - 40) {
            float lengthRatio = videoPivotHeight / 5000;
            float barLength = 250 * lengthRatio;
            if(x > 135 + barLength - 10 && x <= 135 + barLength + 10) {
                panelClickIndex = 2;
                //setProjector(projector);
            }
        }
    }*/
    if(y > 30 && y <= 30 + manufacturerMenu.getHeight() && !modelClicked) {
        if(x > 10 && x <= 10 + manufacturerMenu.getWidth() * 1.5) {
            manuClicked = true;
        }
        else
            manuClicked = false;
    }
    else if(y > 90 && y <= 90 + modelMenu.getHeight() && !manuClicked) {
        if(x > 10 && x <= 10 + modelMenu.getWidth()) {
            modelClicked = true;
        }
        else
            modelClicked = false;
    }
    else {
        manuClicked = false;
        modelClicked = false;
    }
    
    if(manuHoverIndex!=-1) {
        projector->manufacturerName = manufacturerNameList[manuHoverIndex];
        manuHoverIndex = -1;
        manuClicked = false;
        setModelList();
    }
    
    if(modelHoverIndex!=-1) {
        projector->modelName = ofSplitString(modelNameList[modelHoverIndex],",")[1];
        projector->pivotDistance = stof(ofSplitString(modelNameList[modelHoverIndex],",")[2]);
        projector->pivotWidth = stof(ofSplitString(modelNameList[modelHoverIndex],",")[3]);
        projector->pivotHeight = stof(ofSplitString(modelNameList[modelHoverIndex],",")[4]);
        modelHoverIndex = -1;
        modelClicked = false;
    }
}

//--------------------------------------------------------------
void MappingApp::mouseReleased(int x, int y, int button){
    if(panelClickIndex != -1) {
        panelClickIndex = -1;
        setProjector(projector);
    }
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
    //float videoWidth = projector->videoPlayer.getWidth();
    //float videoHeight = projector->videoPlayer.getHeight();
    float videoWidth = projector->width;
    float videoHeight = projector->height;
    
    realCenterPt.x = projector->width/2;
    realCenterPt.y = projector->height/2;
    
    float longerOne,shorterOne;
    float ratio;
    if(videoWidth > videoHeight) {
        /*longerOne = videoWidth;
        shorterOne = videoHeight;
        
        ratio = (float)shorterOne/longerOne;
        
        longerOne = 600;
        shorterOne = 600 * ratio;
        
        videoWidth = longerOne;
        videoHeight = shorterOne;*/
        
        longerOne = videoWidth;
        shorterOne = videoHeight;
        
        if(longerOne > 600) {
            ratio = 600 / longerOne;
            
            longerOne *= ratio;
            shorterOne *= ratio;
            
            videoWidth = longerOne;
            videoHeight = shorterOne;
        }
    }
    else {
        /*longerOne = videoHeight;
        shorterOne = videoWidth;
        
        ratio = (float)shorterOne/longerOne;
        
        longerOne = 600;
        shorterOne = 600 * ratio;
        
        videoWidth = shorterOne;
        videoHeight = longerOne;*/
        
        longerOne = videoHeight;
        shorterOne = videoWidth;
        
        if(longerOne > 600) {
            ratio = 600 / longerOne;
            
            longerOne *= ratio;
            shorterOne *= ratio;
            
            videoHeight = longerOne;
            videoWidth = shorterOne;
        }
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
    
    if(projector->videoPlayer.isLoaded()) {
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

/*void MappingApp::drawPanel() {
    panelWindow.draw(10, ofGetHeight() - 150, 450, 140);
    
    float videoPivotDistance = projector->pivotDistance;
    float videoPivotWidth = projector->pivotWidth;
    float videoPivotHeight = projector->pivotHeight;
    
    ofDrawBitmapString("Pivot Distance", 20, ofGetHeight() - 100);
    ofDrawBitmapString("Pivot Width", 20, ofGetHeight() - 70);
    ofDrawBitmapString("Pivot Height", 20, ofGetHeight() - 40);
    
    ofDrawBitmapString(ofToString(videoPivotDistance), 400, ofGetHeight() - 100);
    ofDrawBitmapString(ofToString(videoPivotWidth), 400, ofGetHeight() - 70);
    ofDrawBitmapString(ofToString(videoPivotHeight), 400, ofGetHeight() - 40);
    
    panelGrayBar.draw(135, ofGetHeight() - 110, 250, 10);
    panelGrayBar.draw(135, ofGetHeight() - 80, 250, 10);
    panelGrayBar.draw(135, ofGetHeight() - 50, 250, 10);
    
    float lengthRatio = videoPivotDistance / 3000;
    float barLength = 250 * lengthRatio;
    panelGreenBar.draw(135, ofGetHeight() - 110, barLength, 10);
    panelSliderButton.draw(135 + barLength - 10, ofGetHeight() - 115, 20, 20);
    lengthRatio = videoPivotWidth / 5000;
    barLength = 250 * lengthRatio;
    panelGreenBar.draw(135, ofGetHeight() - 80, barLength, 10);
    panelSliderButton.draw(135 + barLength - 10, ofGetHeight() - 85, 20, 20);
    lengthRatio = videoPivotHeight / 5000;
    barLength = 250 * lengthRatio;
    panelGreenBar.draw(135, ofGetHeight() - 50, barLength, 10);
    panelSliderButton.draw(135 + barLength - 10, ofGetHeight() - 55, 20, 20);
}*/

void MappingApp::drawChooseMenu() {
    string currentManufacturer = projector->manufacturerName;
    string currentModel = projector->modelName;
    if(!manuClicked) {
        manufacturerMenu.draw(10, 30, manufacturerMenu.getWidth() * 1.5, manufacturerMenu.getHeight());
        if(currentManufacturer.compare("")==0) {
            ofPushStyle();
            ofSetColor(0, 0, 0);
            ofDrawBitmapString("Choose a Manufacturer", 40, 55);
            ofPopStyle();
        }
        else {
            ofPushStyle();
            ofSetColor(0, 0, 0);
            ofDrawBitmapString(projector->manufacturerName, 40, 55);
            ofPopStyle();
        }
    }
    else {
        manufacturerClick.draw(10, 30, manufacturerClick.getWidth() * 1.5, manufacturerClick.getHeight());
        if(currentManufacturer.compare("")==0) {
            ofPushStyle();
            ofSetColor(255, 255, 255);
            ofDrawBitmapString("Choose a Manufacturer", 40, 55);
            ofPopStyle();
        }
        else {
            ofPushStyle();
            ofSetColor(255, 255, 255);
            ofDrawBitmapString(projector->manufacturerName, 40, 55);
            ofPopStyle();
        }
    }
    
    if(!modelClicked) {
        modelMenu.draw(10, 90);
        if(currentModel.compare("")==0) {
            ofPushStyle();
            ofSetColor(0, 0, 0);
            ofDrawBitmapString("Select a model", 30, 115);
            ofPopStyle();
        }
        else {
            ofPushStyle();
            ofSetColor(0, 0, 0);
            ofDrawBitmapString(projector->modelName, 30, 115);
            ofPopStyle();
        }
    }
    else {
        modelClick.draw(10, 90);
        if(currentModel.compare("")==0) {
            ofPushStyle();
            ofSetColor(255, 255, 255);
            ofDrawBitmapString("Select a model", 30, 115);
            ofPopStyle();
        }
        else {
            ofPushStyle();
            ofSetColor(255, 255, 255);
            ofDrawBitmapString(projector->modelName, 30, 115);
            ofPopStyle();
        }
    }
    
    if(modelClicked) {
        for(int i=0; i<modelNameList.size(); i++) {
            if(i==0) {
                if(modelHoverIndex==i)
                    topMenuHover.draw(10, 90 + modelMenu.getHeight(), modelMenu.getWidth(), modelMenu.getHeight());
                else
                    topMenu.draw(10, 90 + modelMenu.getHeight(), modelMenu.getWidth(), modelMenu.getHeight());
            }
            else if(i==modelNameList.size()-1) {
                if(modelHoverIndex==i)
                    bottomMenuHover.draw(10, 90 + modelMenu.getHeight() * (i + 1), modelMenu.getWidth(), modelMenu.getHeight());
                else
                    bottomMenu.draw(10, 90 + modelMenu.getHeight() * (i + 1), modelMenu.getWidth(), modelMenu.getHeight());
            }
            else {
                if(modelHoverIndex==i)
                    centerMenuHover.draw(10, 90 + modelMenu.getHeight() * (i + 1), modelMenu.getWidth(), modelMenu.getHeight());
                else
                    centerMenu.draw(10, 90 + modelMenu.getHeight() * (i + 1), modelMenu.getWidth(), modelMenu.getHeight());
            }
            if(modelHoverIndex==i) {
                ofPushStyle();
                ofSetColor(255, 255, 255);
                ofDrawBitmapString(ofSplitString(modelNameList[i],",")[1], 40, 110 + 40 * (i + 1));
                ofPopStyle();
            }
            else {
                ofPushStyle();
                ofSetColor(0,0,0);
                ofDrawBitmapString(ofSplitString(modelNameList[i],",")[1], 40, 110 + 40 * (i + 1));
                ofPopStyle();
            }
        }
    }
    if(manuClicked) {
        for(int i=0; i<manufacturerNameList.size(); i++) {
            if(i==0) {
                if(manuHoverIndex==i)
                    topMenuHover.draw(10, 30 + manufacturerMenu.getHeight(), manufacturerMenu.getWidth() * 1.5, manufacturerMenu.getHeight());
                else
                    topMenu.draw(10, 30 + manufacturerMenu.getHeight(), manufacturerMenu.getWidth() * 1.5, manufacturerMenu.getHeight());
            }
            else if(i==manufacturerNameList.size()-1) {
                if(manuHoverIndex==i)
                    bottomMenuHover.draw(10, 30 + manufacturerMenu.getHeight() * (i + 1), manufacturerMenu.getWidth() * 1.5, manufacturerMenu.getHeight());
                else
                bottomMenu.draw(10, 30 + manufacturerMenu.getHeight() * (i + 1), manufacturerMenu.getWidth() * 1.5, manufacturerMenu.getHeight());
            }
            else {
                if(manuHoverIndex==i)
                    centerMenuHover.draw(10, 30 + manufacturerMenu.getHeight() * (i + 1), manufacturerMenu.getWidth() * 1.5, manufacturerMenu.getHeight());
                else
                    centerMenu.draw(10, 30 + manufacturerMenu.getHeight() * (i + 1), manufacturerMenu.getWidth() * 1.5, manufacturerMenu.getHeight());
            }
            if(manuHoverIndex==i) {
                ofPushStyle();
                ofSetColor(255, 255, 255);
                ofDrawBitmapString(manufacturerNameList[i], 40, 50 + 40 * (i + 1));
                ofPopStyle();
            }
            else {
                ofPushStyle();
                ofSetColor(0,0,0);
                ofDrawBitmapString(manufacturerNameList[i], 40, 50 + 40 * (i + 1));
                ofPopStyle();
            }
        }
    }
}

void MappingApp::setModelList() {
    modelNameList.clear();
    projector->modelName = "";
    for(int i=0; i<specList.size(); i++) {
        if(ofSplitString(specList[i],",")[0].compare(projector->manufacturerName)==0) {
            modelNameList.push_back(specList[i]);
        }
    }
}
