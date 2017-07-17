#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(70,70,70);
    ofEnableDepthTest(); //draw in order of z value
    
    //allocating new projector class - test case : 3 projectors
    projectors.push_back(new Projector(0,20,150,0,0,0,projectors.size()));
    projectors.push_back(new Projector(200,20,150,0,330,0,projectors.size()));
    projectors.push_back(new Projector(-200,20,150,0,30,0,projectors.size()));
    
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    //MUST toggle off the ofDisableDepthTest() before draw the gui
    ofDisableDepthTest();
    gui.draw();
    ofEnableDepthTest();
    
    easyCam.begin();
    
    ofDrawGrid(100.f);
    
    string text = "Current selected Projector : ";
    for(int i=0; i<projectors.size(); i++) {
        projectors[i]->draw();
        if(projectors[i]->isSelected)
            text += ofToString(projectors[i]->projectorNum);
    }
    
    easyCam.end();
    ofDrawBitmapString(text,10,10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 'd' : //delete the selected projector
            for(int i=0; i<projectors.size(); i++) {
                if(projectors[i]->isSelected)
                    deleteProjector(i--);
            }
            break;
        default : break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //projector object mouse picking
    float nearestDistance = 50;
    int nearestIndex = -1;
    ofVec2f mouse(x,y);
    for(int i=0; i<projectors.size(); i++) {
        ofVec3f projectorPt = easyCam.worldToScreen(ofVec3f(projectors[i]->xPos, projectors[i]->yPos, projectors[i]->zPos));
        float distance = projectorPt.distance(mouse);
        if(distance < 50 && distance < nearestDistance) {
            nearestDistance = distance;
            nearestIndex = i;
        }
    }
    if(nearestIndex != -1) {
        projectors[nearestIndex]->isSelected = !projectors[nearestIndex]->isSelected;
        refreshGUI();
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//Additional function
void ofApp::refreshGUI() {
    gui.clear();
    projectorPanelGroup.clear();
    for(int i=0; i<projectors.size(); i++) {
        if(projectors[i]->isSelected) {
            projectorPanelGroup.add(projectors[i]->projectorParameters);
        }
    }
    gui.setup(projectorPanelGroup);
}

void ofApp::deleteProjector(int projectorNum) {
    projectors.erase(projectors.begin() + projectorNum);
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->projectorNum = i;
    refreshGUI();
}
