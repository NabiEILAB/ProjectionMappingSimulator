#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(70,70,70);
    ofEnableDepthTest(); //draw in order of z value
    
    //allocating new projector class - test case : 3 projectors moved to keyEvent
    projectors.push_back(new Projector(-400,20,850,0,0,0,projectors.size()));
    projectors.push_back(new Projector(-300,20,850,0,0,0,projectors.size()));
    projectors.push_back(new Projector(-200,20,850,0,0,0,projectors.size()));
    projectors.push_back(new Projector(-100,20,850,0,0,0,projectors.size()));
    projectors.push_back(new Projector(0,20,850,0,0,0,projectors.size()));
    projectors.push_back(new Projector(100,20,850,0,0,0,projectors.size()));
    projectors.push_back(new Projector(200,20,850,0,0,0,projectors.size()));
    //projectors.push_back(new Projector(300,20,850,0,0,0,projectors.size()));
    //projectors.push_back(new Projector(400,20,850,0,0,0,projectors.size()));
    
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->setup();
    
    //load shaders
    textureProjectionShader.load("TextureProjection");
    depthStoringShader.load("DepthStoring");
    
    currentSelectedProjector = -1;
    
    gui.setup();
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
    
    ofDrawGrid(200, 15, true, false, true, false);
    
    renderCustomModel();
    
    for(int i=0; i<projectors.size(); i++) {
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
        case 'd' : { //delete the selected projector
            deleteProjector(currentSelectedProjector);
            break;
        }
        case 'f' : {
            ofFileDialogResult openFileResult = ofSystemLoadDialog("Select 3d file");
            
            if(openFileResult.bSuccess)
                open3DFile(openFileResult);
            
            break;
        }
        case 'p' : {
            addProjector();
            break;
        }
        case 'v' : {
            if(currentSelectedProjector==-1)
                break;
            
            ofFileDialogResult openFileResult = ofSystemLoadDialog("Select Video file");
            
            if(openFileResult.bSuccess)
                openVideoFile(openFileResult);
            
            break;
        }
        default : {
            break;
        }
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
            if(projectors[i]->isSetted)
                nearestIndex = i;
        }
    }
    if(nearestIndex != -1) {
        if(currentSelectedProjector == nearestIndex) {
            projectors[nearestIndex]->isSelected = false;
            currentSelectedProjector = -1;
            
            mappingGUI->projector = NULL;
        }
        else {
            if(currentSelectedProjector != -1)
                projectors[currentSelectedProjector]->isSelected = false;
            
            projectors[nearestIndex]->isSelected = true;
            currentSelectedProjector = nearestIndex;
            
            mappingGUI->setProjector(projectors[nearestIndex]);
        }
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

void ofApp::addProjector() {
    for(int i=0; i<projectors.size(); i++) {
        if(!projectors[i]->isSetted) {
            projectors[i]->isSetted = true;
            break;
        }
    }
}

void ofApp::deleteProjector(int projectorNum) {
    projectors[projectorNum]->deactivate();
    refreshGUI();
}

void ofApp::renderCustomModel() {
    for(int i=0; i<projectors.size(); i++) {
        projectors[i]->draw();
        projectors[i]->shadowFbo.begin();
        projectors[i]->clearShadowFbo();
        
        depthStoringShader.begin();
        depthStoringShader.setUniformMatrix4f("projectorProjectionMatrix", projectors[i]->projectorProjection);
        depthStoringShader.setUniformMatrix4f("projectorViewMatrix", projectors[i]->projectorView);
        
        ofMatrix4x4 modelMatrix = ofMatrix4x4::newIdentityMatrix();
        modelMatrix.translate(0, 0, 0);
        
        depthStoringShader.setUniformMatrix4f("modelMatrix", modelMatrix);
        
        glCullFace(GL_FRONT);
        mesh.draw();
        glCullFace(GL_BACK);
        depthStoringShader.end();
        projectors[i]->shadowFbo.end();
        
        setProjectorShader(i);
    }
    
    textureProjectionShader.begin();
    ofMatrix4x4 modelMatrix = ofMatrix4x4::newIdentityMatrix();
    modelMatrix.translate(0, 0, 0);
    
    ofMatrix4x4 bias = ofMatrix4x4::newIdentityMatrix();
    bias.scale(0.5, 0.5, 0.5);
    bias.translate(0.5, 0.5, 0.5);
    
    textureProjectionShader.setUniformMatrix4f("biasMatrix", bias);
    textureProjectionShader.setUniformMatrix4f("modelMatrix", modelMatrix);
    textureProjectionShader.setUniform3f("lightPos", easyCam.getX(), easyCam.getY(), easyCam.getZ());
    
    mesh.draw();
    textureProjectionShader.end();
}

void ofApp::open3DFile(ofFileDialogResult openFileResult) {
    ofFile file(openFileResult.getPath());
    
    if(file.exists()) {
        string fileExtension = ofToUpper(file.getExtension());
        
        if(fileExtension == "3DS" || fileExtension == "ASE" || fileExtension == "DXF" || fileExtension == "HMP" ||
           fileExtension == "MD2" || fileExtension == "MD3" || fileExtension == "MD5" || fileExtension == "MDC" ||
           fileExtension == "MDL" || fileExtension == "NFF" || fileExtension == "PLY" || fileExtension == "STL" ||
           fileExtension == "X" || fileExtension == "LWO" || fileExtension == "OBJ" || fileExtension == "SMD" ||
           fileExtension == "COLLADA") {
            model.loadModel(openFileResult.getPath());
        }
        
        model.setPosition(0, 250, 0);
        model.setScale(5, 5, 5);
        model.setRotation(0, 180, 0, 0, 1);
        //model.setRotation(1, 50, 0, 1, 0);
        
        reconstructMesh();
    }
}

void ofApp::reconstructMesh() {
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
        norm.set(0, 0, 0);
    }
    
    //Reconstruct normal vector
    for(int i = 0; i < mesh.getIndices().size(); i += 3) {
        int aIndex = mesh.getIndices()[i];
        int bIndex = mesh.getIndices()[i + 1];
        int cIndex = mesh.getIndices()[i + 2];
        
        ofVec3f offset1 = mesh.getVertices()[aIndex] - mesh.getVertices()[bIndex];
        ofVec3f offset2 = mesh.getVertices()[cIndex] - mesh.getVertices()[bIndex];
        ofVec3f normalValue = offset2.cross(offset1);
        
        mesh.getNormals()[aIndex] += normalValue;
        mesh.getNormals()[bIndex] += normalValue;
        mesh.getNormals()[cIndex] += normalValue;
    }
}

void ofApp::openVideoFile(ofFileDialogResult openFileResult) {
    ofFile file(openFileResult.getPath());
    
    if(file.exists()) {
        string fileExtension = ofToUpper(file.getExtension());
        
        if(fileExtension == "MOV" || fileExtension == "MP4" || fileExtension == "AVI") {
            projectors[currentSelectedProjector]->videoPlayer.load(openFileResult.getPath());
            projectors[currentSelectedProjector]->videoPlayer.play();
            mappingGUI->setProjector(projectors[currentSelectedProjector]);
        }
    }
}

void ofApp::setProjectorShader(int index) {
    textureProjectionShader.begin();
    if(index==0){
        textureProjectionShader.setUniformMatrix4f("projectorMatrix1", projectors[index]->projectorMatrix);
        textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix1", projectors[index]->projectorProjection);
        textureProjectionShader.setUniformMatrix4f("projectorViewMatrix1", projectors[index]->projectorView);
        textureProjectionShader.setUniform3f("projectorPos1", projectors[index]->xPos, projectors[index]->yPos, projectors[index]->zPos);
        textureProjectionShader.setUniformTexture("projectorTex1", projectors[index]->textureFbo.getTexture(), 16);
        textureProjectionShader.setUniformTexture("shadowTex1", projectors[index]->shadowFbo.getDepthTexture(), 17);
        textureProjectionShader.setUniform1f("radius1", projectors[index]->height); //radius att test!
        if(projectors[index]->isSetted && projectors[index]->videoPlayer.isLoaded()) {
            textureProjectionShader.setUniform1f("isSet1",1);
        }
        else
            textureProjectionShader.setUniform1f("isSet1",0);
    }
    else if(index==1) {
        textureProjectionShader.setUniformMatrix4f("projectorMatrix2", projectors[index]->projectorMatrix);
        textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix2", projectors[index]->projectorProjection);
        textureProjectionShader.setUniformMatrix4f("projectorViewMatrix2", projectors[index]->projectorView);
        textureProjectionShader.setUniform3f("projectorPos2", projectors[index]->xPos, projectors[index]->yPos, projectors[index]->zPos);
        textureProjectionShader.setUniformTexture("projectorTex2", projectors[index]->textureFbo.getTexture(), 14);
        textureProjectionShader.setUniformTexture("shadowTex2", projectors[index]->shadowFbo.getDepthTexture(), 15);
        textureProjectionShader.setUniform1f("radius2", projectors[index]->height);
        if(projectors[index]->isSetted && projectors[index]->videoPlayer.isLoaded())
            textureProjectionShader.setUniform1f("isSet2",1);
        else
            textureProjectionShader.setUniform1f("isSet2",0);
    }
    else if(index==2) {
        textureProjectionShader.setUniformMatrix4f("projectorMatrix3", projectors[index]->projectorMatrix);
        textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix3", projectors[index]->projectorProjection);
        textureProjectionShader.setUniformMatrix4f("projectorViewMatrix3", projectors[index]->projectorView);
        textureProjectionShader.setUniform3f("projectorPos3", projectors[index]->xPos, projectors[index]->yPos, projectors[index]->zPos);
        textureProjectionShader.setUniformTexture("projectorTex3", projectors[index]->textureFbo.getTexture(), 12);
        textureProjectionShader.setUniformTexture("shadowTex3", projectors[index]->shadowFbo.getDepthTexture(), 13);
        textureProjectionShader.setUniform1f("radius3", projectors[index]->height);
        if(projectors[index]->isSetted && projectors[index]->videoPlayer.isLoaded())
            textureProjectionShader.setUniform1f("isSet3",1);
        else
            textureProjectionShader.setUniform1f("isSet3",0);
    }
    else if(index==3) {
        textureProjectionShader.setUniformMatrix4f("projectorMatrix4", projectors[index]->projectorMatrix);
        textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix4", projectors[index]->projectorProjection);
        textureProjectionShader.setUniformMatrix4f("projectorViewMatrix4", projectors[index]->projectorView);
        textureProjectionShader.setUniform3f("projectorPos4", projectors[index]->xPos, projectors[index]->yPos, projectors[index]->zPos);
        textureProjectionShader.setUniformTexture("projectorTex4", projectors[index]->textureFbo.getTexture(), 10);
        textureProjectionShader.setUniformTexture("shadowTex4", projectors[index]->shadowFbo.getDepthTexture(), 11);
        textureProjectionShader.setUniform1f("radius4", projectors[index]->height);
        if(projectors[index]->isSetted && projectors[index]->videoPlayer.isLoaded())
            textureProjectionShader.setUniform1f("isSet4",1);
        else
            textureProjectionShader.setUniform1f("isSet4",0);
    }
    else if(index==4) {
        textureProjectionShader.setUniformMatrix4f("projectorMatrix5", projectors[index]->projectorMatrix);
        textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix5", projectors[index]->projectorProjection);
        textureProjectionShader.setUniformMatrix4f("projectorViewMatrix5", projectors[index]->projectorView);
        textureProjectionShader.setUniform3f("projectorPos5", projectors[index]->xPos, projectors[index]->yPos, projectors[index]->zPos);
        textureProjectionShader.setUniformTexture("projectorTex5", projectors[index]->textureFbo.getTexture(), 8);
        textureProjectionShader.setUniformTexture("shadowTex5", projectors[index]->shadowFbo.getDepthTexture(), 9);
        textureProjectionShader.setUniform1f("radius5", projectors[index]->height);
        if(projectors[index]->isSetted && projectors[index]->videoPlayer.isLoaded())
            textureProjectionShader.setUniform1f("isSet5",1);
        else
            textureProjectionShader.setUniform1f("isSet5",0);
    }
    else if(index==5) {
        textureProjectionShader.setUniformMatrix4f("projectorMatrix6", projectors[index]->projectorMatrix);
        textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix6", projectors[index]->projectorProjection);
        textureProjectionShader.setUniformMatrix4f("projectorViewMatrix6", projectors[index]->projectorView);
        textureProjectionShader.setUniform3f("projectorPos6", projectors[index]->xPos, projectors[index]->yPos, projectors[index]->zPos);
        textureProjectionShader.setUniformTexture("projectorTex6", projectors[index]->textureFbo.getTexture(), 6);
        textureProjectionShader.setUniformTexture("shadowTex6", projectors[index]->shadowFbo.getDepthTexture(), 7);
        textureProjectionShader.setUniform1f("radius6", projectors[index]->height);
        if(projectors[index]->isSetted && projectors[index]->videoPlayer.isLoaded())
            textureProjectionShader.setUniform1f("isSet6",1);
        else
            textureProjectionShader.setUniform1f("isSet6",0);
    }
    else if(index==6) {
        textureProjectionShader.setUniformMatrix4f("projectorMatrix7", projectors[index]->projectorMatrix);
        textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix7", projectors[index]->projectorProjection);
        textureProjectionShader.setUniformMatrix4f("projectorViewMatrix7", projectors[index]->projectorView);
        textureProjectionShader.setUniform3f("projectorPos7", projectors[index]->xPos, projectors[index]->yPos, projectors[index]->zPos);
        textureProjectionShader.setUniformTexture("projectorTex7", projectors[index]->textureFbo.getTexture(), 4);
        textureProjectionShader.setUniformTexture("shadowTex7", projectors[index]->shadowFbo.getDepthTexture(), 5);
        textureProjectionShader.setUniform1f("radius7", projectors[index]->height);
        if(projectors[index]->isSetted && projectors[index]->videoPlayer.isLoaded())
            textureProjectionShader.setUniform1f("isSet7",1);
        else
            textureProjectionShader.setUniform1f("isSet7",0);
    }
    /*else if(index==7) {
        textureProjectionShader.setUniformMatrix4f("projectorMatrix8", projectors[index]->projectorMatrix);
        textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix8", projectors[index]->projectorProjection);
        textureProjectionShader.setUniformMatrix4f("projectorViewMatrix8", projectors[index]->projectorView);
        textureProjectionShader.setUniform3f("projectorPos8", projectors[index]->xPos, projectors[index]->yPos, projectors[index]->zPos);
        textureProjectionShader.setUniformTexture("projectorTex8", projectors[index]->textureFbo.getTexture(), 2);
        textureProjectionShader.setUniformTexture("shadowTex8", projectors[index]->shadowFbo.getDepthTexture(), 3);
        textureProjectionShader.setUniform1f("radius8", projectors[index]->height);
        if(projectors[index]->isSetted && projectors[index]->videoPlayer.isLoaded())
            textureProjectionShader.setUniform1f("isSet8",1);
        else
            textureProjectionShader.setUniform1f("isSet8",0);
    }
    else if(index==8) {
        textureProjectionShader.setUniformMatrix4f("projectorMatrix9", projectors[index]->projectorMatrix);
        textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix9", projectors[index]->projectorProjection);
        textureProjectionShader.setUniformMatrix4f("projectorViewMatrix9", projectors[index]->projectorView);
        textureProjectionShader.setUniform3f("projectorPos9", projectors[index]->xPos, projectors[index]->yPos, projectors[index]->zPos);
        textureProjectionShader.setUniformTexture("projectorTex9", projectors[index]->textureFbo.getTexture(), 0);
        textureProjectionShader.setUniformTexture("shadowTex9", projectors[index]->shadowFbo.getDepthTexture(), 1);
        textureProjectionShader.setUniform1f("radius9", projectors[index]->height);
        if(projectors[index]->isSetted && projectors[index]->videoPlayer.isLoaded())
            textureProjectionShader.setUniform1f("isSet9",1);
        else
            textureProjectionShader.setUniform1f("isSet9",0);
     }*/
    textureProjectionShader.end();
}
