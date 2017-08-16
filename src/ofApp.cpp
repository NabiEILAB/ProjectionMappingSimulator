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
    
    //////////////////   Load model test   ///////////////////
    model.loadModel("CardBoardBox.3ds");
    model.setPosition(0, 25, 0);
    model.setScale(1, 1, 1);
    model.setRotation(0, 180, 1, 0, 0);
    model.setRotation(1, 45, 0, 1, 0);
    
    //Model itself can't adjust texturing... I don't know why. this code extracts mesh from the model
    ofMatrix4x4 modelMatrix = model.getModelMatrix();
    ofMatrix4x4 meshMatrix = model.getMeshHelper(0).matrix;
    ofMatrix4x4 concatMatrix;
    concatMatrix.preMult(modelMatrix);
    concatMatrix.preMult(meshMatrix);
    
    //Reconstruct mesh's vertices and normals from the model object
    mesh = model.getMesh(0);
    for(int i=0; i<mesh.getNumVertices(); i++) {
        ofVec3f& vert = mesh.getVertices()[i];
        vert.set(concatMatrix.preMult(vert));
        ofVec3f& norm = mesh.getNormals()[i];
        norm.set(concatMatrix.preMult(norm));
    }
    //////////////////////////////////////////////////////////
    
    
    ///////     Draw video on primitive object test    ///////
    plane.set(640,480);
    plane.setPosition(0,0,0);
    box.set(350,185,20);
    box.setPosition(-150,20,150);
    box.rotate(33, 0, 1, 0);
    box2.set(30,45,80);
    box2.setPosition(120,15,120);
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
    //////////////////////////////////////////////////////////
    
    
    //load shaders
    textureProjectionShader.load("TextureProjection");
    depthStoringShader.load("DepthStoring");
    
    //allocate Frame Buffer Object
    allocateFbo();
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    easyCam.begin();
    
    string text = "Current selected Projector : ";
    for(int i=0; i<projectors.size(); i++) {
        projectors[i]->draw();
        
        //test case : draw on custom model
        
        //Make depth map - render the scene in projector's view to FBO
        /*shadowFbo.begin();
            clearFbo();
            depthStoringShader.begin();
                depthStoringShader.setUniformMatrix4f("projectorProjectionMatrix", projectors[i]->projectorProjection);
                depthStoringShader.setUniformMatrix4f("projectorViewMatrix", projectors[i]->projectorView);
                ofMatrix4x4 modelMatrix = ofMatrix4x4::newIdentityMatrix();
                depthStoringShader.setUniformMatrix4f("modelMatrix", modelMatrix);
        
                glCullFace(GL_FRONT);
                mesh.draw();
                glCullFace(GL_BACK);
        
            depthStoringShader.end();
        shadowFbo.end();
        
        //Render the scene in camera's view
        textureProjectionShader.begin();
            textureProjectionShader.setUniformMatrix4f("projectorMatrix", projectors[i]->projectorMatrix);
            textureProjectionShader.setUniformTexture("projectorTex", projectors[i]->videoTexture, 0);
            textureProjectionShader.setUniformTexture("shadowTex", shadowFbo.getDepthTexture(), 1);
        
            ofMatrix4x4 biasMatrix = ofMatrix4x4::newIdentityMatrix();
            biasMatrix.scale(0.5, 0.5, 0.5);
            biasMatrix.translate(0.5, 0.5, 0.5);
            textureProjectionShader.setUniformMatrix4f("biasMatrix", biasMatrix);
            textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix", projectors[i]->projectorProjection);
            textureProjectionShader.setUniformMatrix4f("projectorViewMatrix", projectors[i]->projectorView);
            textureProjectionShader.setUniformMatrix4f("modelMatrix", modelMatrix);
            textureProjectionShader.setUniform3f("projectorPos", ofVec3f(projectors[i]->xPos, projectors[i]->yPos, projectors[i]->zPos));
        
            mesh.draw();
        textureProjectionShader.end();*/
        
        
        
        
        //test case : draw on primitive objects
        shadowFbo.begin();
            clearFbo();
            depthStoringShader.begin();
                depthStoringShader.setUniformMatrix4f("projectorProjectionMatrix", projectors[i]->projectorProjection);
                depthStoringShader.setUniformMatrix4f("projectorViewMatrix", projectors[i]->projectorView);
            
                glCullFace(GL_FRONT);
                for(int j=0; j<modelAttributes.size(); j++) {
                    ofMatrix4x4 modelMatrix = ofMatrix4x4::newIdentityMatrix();
                    if(j==1)
                        modelMatrix.rotate(33, 0, 1, 0);
                    modelMatrix.translate(modelAttributes[j].x, modelAttributes[j].y, modelAttributes[j].z);
                    depthStoringShader.setUniformMatrix4f("modelMatrix", modelMatrix);
                    models[j].draw();
                }
                glCullFace(GL_BACK);
            depthStoringShader.end();
        shadowFbo.end();
        
        
        textureProjectionShader.begin();
            textureProjectionShader.setUniformMatrix4f("projectorMatrix", projectors[i]->projectorMatrix);
            textureProjectionShader.setUniformTexture("projectorTex", projectors[i]->videoTexture, 0);
            textureProjectionShader.setUniformTexture("shadowTex", shadowFbo.getDepthTexture(), 1);
        
            ofMatrix4x4 biasMatrix = ofMatrix4x4::newIdentityMatrix();
            biasMatrix.scale(0.5, 0.5, 0.5);
            biasMatrix.translate(0.5, 0.5, 0.5);
            textureProjectionShader.setUniformMatrix4f("biasMatrix", biasMatrix);
            textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix", projectors[i]->projectorProjection);
            textureProjectionShader.setUniformMatrix4f("projectorViewMatrix", projectors[i]->projectorView);
            textureProjectionShader.setUniform3f("projectorPos", ofVec3f(projectors[i]->xPos, projectors[i]->yPos, projectors[i]->zPos));
        
            for(int j=0; j<modelAttributes.size(); j++) {
                ofMatrix4x4 modelMatrix = ofMatrix4x4::newIdentityMatrix();
                if(j==1)
                    modelMatrix.rotate(33, 0, 1, 0);
                modelMatrix.translate(modelAttributes[j].x, modelAttributes[j].y, modelAttributes[j].z);
                textureProjectionShader.setUniformMatrix4f("modelMatrix", modelMatrix);
            
                models[j].draw();
            }
        textureProjectionShader.end();
        
        projectors[i]->videoTexture.clear();
        if(projectors[i]->isSelected)
            text += ofToString(projectors[i]->projectorNum);
    }
    
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

void ofApp::allocateFbo() {
    ofFbo::Settings settings;
    settings.width = 1024;
    settings.height = 768;
    settings.textureTarget = GL_TEXTURE_2D;
    settings.internalformat = GL_RGBA32F_ARB;
    settings.useDepth = true;
    settings.depthStencilAsTexture = true;
    settings.useStencil = true;
    shadowFbo.allocate(settings);
}

void ofApp::clearFbo() {
    ofClear(0,0,0,1);
    ofSetColor(255);
    shadowFbo.getDepthTexture().draw(0,0);
    glClear(GL_DEPTH_BUFFER_BIT);
}
