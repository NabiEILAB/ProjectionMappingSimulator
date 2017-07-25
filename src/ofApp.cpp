#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(70,70,70);
    ofEnableDepthTest(); //draw in order of z value
    
    //allocating new projector class - test case : 3 projectors
    projectors.push_back(new Projector(0,20,450,0,0,0,projectors.size()));
    //projectors.push_back(new Projector(200,20,450,0,330,0,projectors.size()));
    //projectors.push_back(new Projector(-200,20,450,0,30,0,projectors.size()));
    
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->setup();
    
    //Load model test
    /*model.loadModel("CardBoardBox.3ds",false);
    model.setPosition(0, 100, 0);
    model.setScale(1, 1, 1);
    model.setRotation(0, 180, 1, 0, 0);
    model.setRotation(1, 45, 0, 1, 0);*/
    
    //Draw video test
    plane.set(640,480);
    plane.setPosition(0,0,0);
    box.set(350,185,20);
    box.setPosition(-150,20,150);
    box2.set(30,45,80);
    box2.setPosition(120,15,50);
    ball.setRadius(50);
    ball.setPosition(150, 100, 150);
    
    models.push_back(plane);
    models.push_back(box);
    models.push_back(box2);
    models.push_back(ball);
    
    modelAttributes.push_back(ofVec3f(0,0,0));
    modelAttributes.push_back(ofVec3f(-150,20,150));
    modelAttributes.push_back(ofVec3f(120,15,50));
    modelAttributes.push_back(ofVec3f(150,100,150));
    
    //load texture shader
    textureProjectionShader.load("TextureProjection");
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    //MUST toggle off the ofDisableDepthTest() before draw the gui
    //ofDisableDepthTest();
    //gui.draw();
    //ofEnableDepthTest();
    
    easyCam.begin();
    
    //ofDrawGrid(100.f);
    
    textureProjectionShader.begin();
    string text = "Current selected Projector : ";
    for(int i=0; i<projectors.size(); i++) {
        projectors[i]->draw();
        //test case : one model in 0,0,0 - no need to translate modelMatrix
        for(int j=0; j<modelAttributes.size(); j++) {
            ofMatrix4x4 modelMatrix = ofMatrix4x4::newIdentityMatrix();
            modelMatrix.translate(modelAttributes[j].x, modelAttributes[j].y, modelAttributes[j].z);
            //projection texture input
            textureProjectionShader.setUniformMatrix4f("modelMatrix", modelMatrix);
            textureProjectionShader.setUniformMatrix4f("projectorMatrix", projectors[i]->projectorMatrix);
            textureProjectionShader.setUniformTexture("projectorTex", projectors[i]->videoTexture, 0);
            
            models[j].draw();
        }
        if(projectors[i]->isSelected)
            text += ofToString(projectors[i]->projectorNum);
    }
    
    textureProjectionShader.end();
    
    //model.drawFaces();
    
    easyCam.end();
    ofDrawBitmapString(text,10,10);
    
    ofDisableDepthTest();
    gui.draw();
    ofEnableDepthTest();
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
