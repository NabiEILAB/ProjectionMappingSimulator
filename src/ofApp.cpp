#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(70,70,70);
    ofEnableDepthTest(); //draw in order of z value
    
    //allocating new projector class - 7 projectors
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    //projectors.push_back(new Projector(300,20,850,0,0,0,projectors.size()));
    //projectors.push_back(new Projector(400,20,850,0,0,0,projectors.size()));
    
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->setup();
    
    //load shaders
    textureProjectionShader.load("TextureProjection");
    depthStoringShader.load("DepthStoring");
    projectorTextureShader.load("ProjectorTexture");
    
    currentSelectedProjector = -1;
    //gui.setup();
    
    //UI resource setup
    gridDropDown[0].load("UI/MouseDropDown/1.makeProjector.png");
    gridDropDownHover[0].load("UI/MouseDropDown/2.makeProjectorHover.png");
    gridDropDownHoverIndex = -1;
    isGridDropDownOn = false;
    
    projectorDropDown[0].load("UI/MouseDropDown/5.videoLoad.png");
    projectorDropDown[1].load("UI/MouseDropDown/3.deleteProjector.png");
    projectorDropDownHover[0].load("UI/MouseDropDown/6.videoLoadHover.png");
    projectorDropDownHover[1].load("UI/MouseDropDown/4.deleteProjectorHover.png");
    projectorDropDownHoverIndex = -1;
    isProjectorDropDownOn = false;
    
    headerMenu[0].load("UI/UpperMenu/1.file.png");
    headerMenu[1].load("UI/UpperMenu/3.modeling.png");
    headerHover[0].load("UI/UpperMenu/2.fileClick.png");
    headerHover[1].load("UI/UpperMenu/4.modelingClick.png");
    headerHoverIndex = -1;
    
    subMenuFile[0].load("UI/UpperMenu/5.newProject.png");
    subMenuFile[1].load("UI/UpperMenu/7.saveProject.png");
    subMenuFile[2].load("UI/UpperMenu/9.loadProject.png");
    subFileHover[0].load("UI/UpperMenu/6.newProjectHover.png");
    subFileHover[1].load("UI/UpperMenu/8.saveProjectHover.png");
    subFileHover[2].load("UI/UpperMenu/10.loadProjectHover.png");
    subFileHoverIndex = -1;
    
    subMenuModeling[0].load("UI/UpperMenu/11.loadModeling.png");
    subModelingHover[0].load("UI/UpperMenu/12.loadModelingHover.png");
    subModelingHoverIndex = -1;
    
    cameraButton[0].load("UI/Button/+ deselect.png");
    cameraButton[1].load("UI/Button/- deselect.png");
    cameraButton[2].load("UI/Button/left deselect.png");
    cameraButton[3].load("UI/Button/right deselect.png");
    cameraButton[4].load("UI/Button/up deselect.png");
    cameraButton[5].load("UI/Button/down deselect.png");
    cameraButtonPressed[0].load("UI/Button/+ select.png");
    cameraButtonPressed[1].load("UI/Button/- select.png");
    cameraButtonPressed[2].load("UI/Button/left select.png");
    cameraButtonPressed[3].load("UI/Button/right select.png");
    cameraButtonPressed[4].load("UI/Button/up select.png");
    cameraButtonPressed[5].load("UI/Button/down select.png");
    cameraButtonPressedIndex = -1;
    
    panelWindow.load("UI/Panel/panelWindow.png");
    panelGrayBar.load("UI/Panel/grayBar.png");
    panelGreenBar.load("UI/Panel/greenBar.png");
    panelSliderButton.load("UI/Panel/sliderButton.png");
    panelLabel.load("UI/Panel/label.png");
    panelLabelClick.load("UI/Panel/labelClick.png");
    panelClickIndex = -1;
    
    currentModelURL = "";
    
    projectorModel.loadModel("projector.3ds");
    reconstructProjectorMesh();
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    easyCam.begin();
    
    if(cameraButtonPressedIndex==0) {
        easyCam.disableMouseInput();
        easyCam.dolly(-10);
    }
    else if(cameraButtonPressedIndex==1) {
        easyCam.disableMouseInput();
        easyCam.dolly(10);
    }
    else if(cameraButtonPressedIndex==2) {
        easyCam.disableMouseInput();
        easyCam.move(-10, 0, 0);
        easyCam.setTarget(ofVec3f(easyCam.getTarget().getX() - 10,easyCam.getTarget().getY(), easyCam.getTarget().getZ()));
    }
    else if(cameraButtonPressedIndex==3) {
        easyCam.disableMouseInput();
        easyCam.move(10, 0, 0);
        easyCam.setTarget(ofVec3f(easyCam.getTarget().getX() + 10,easyCam.getTarget().getY(), easyCam.getTarget().getZ()));
    }
    else if(cameraButtonPressedIndex==4) {
        easyCam.disableMouseInput();
        easyCam.move(0, 10, 0);
        easyCam.setTarget(ofVec3f(easyCam.getTarget().getX(), easyCam.getTarget().getY() + 10, easyCam.getTarget().getZ()));
    }
    else if(cameraButtonPressedIndex==5) {
        easyCam.disableMouseInput();
        easyCam.move(0, -10, 0);
        easyCam.setTarget(ofVec3f(easyCam.getTarget().getX(), easyCam.getTarget().getY() - 10, easyCam.getTarget().getZ()));
    }
    else {
        if(!easyCam.getMouseInputEnabled() && panelClickIndex == -1) {
            ofNode t = easyCam.getTarget();
            ofVec3f p = easyCam.getPosition();
            easyCam.enableMouseInput();
            easyCam.reset();
            easyCam.setPosition(p.x, p.y, p.z);
            easyCam.setTarget(t);
        }
    }
    
    ofDisableDepthTest();
    ofDrawGrid(200, 15, true, false, true, false);
    ofEnableDepthTest();
    
    renderCustomModel();
    
    //render projector modeling
    projectorTextureShader.begin();
    projectorTextureShader.setUniform3f("lightPos", easyCam.getX(), easyCam.getY(), easyCam.getZ());
    for(int i=0; i<projectors.size(); i++) {
        if(projectors[i]->isSetted) {
            projectorModel.setScale(0.3, 0.3, 0.3);
            projectorModel.setPosition(projectors[i]->xPos, projectors[i]->yPos, projectors[i]->zPos);
            projectorModel.setRotation(0, -projectors[i]->xRotation, 1, 0, 0);
            projectorModel.setRotation(1, -projectors[i]->yRotation, 0, 1, 0);
            projectorModel.setRotation(2, projectors[i]->zRotation + 180, 0, 0, 1);
            reconstructProjectorMesh();
            
            ofMatrix4x4 mat = ofMatrix4x4::newIdentityMatrix();
            mat.translate(projectors[i]->xPos, projectors[i]->yPos, projectors[i]->zPos);
            //mat.rotate(-180, 0, 0, 1);
            projectorTextureShader.setUniformMatrix4f("modelMatrix", mat);
            projectorTextureShader.setUniform1f("isSelected", 0.0);
            projectorMesh.draw();
            if(projectors[i]->isSelected) {
                projectorModel.setScale(0.301, 0.301, 0.301);
                reconstructProjectorMesh();
                projectorTextureShader.setUniform1f("isSelected", 1.0);
                projectorMesh.drawWireframe();
            }
        }
    }
    projectorTextureShader.end();
    
    easyCam.end();
    
    ofDisableDepthTest();
    //gui.draw();
    drawPanel();
    ofEnableDepthTest();
    
    drawHeaders();
    drawButtons();
    drawDropDownMenus();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if(isGridDropDownOn || isProjectorDropDownOn) {
        if(x > clickedCoord.x && x < clickedCoord.x + 144) {
            if(isGridDropDownOn) {
                gridDropDownHoverIndex = -1;
                for(int i = 0; i < sizeof(gridDropDown)/sizeof(gridDropDown[0]); i++) {
                    if(y > clickedCoord.y + (20 * i) && y <= clickedCoord.y + 20 + (20 * i)) {
                        gridDropDownHoverIndex = i;
                        break;
                    }
                }
            }
            else if(isProjectorDropDownOn) {
                projectorDropDownHoverIndex = -1;
                for(int i = 0; i < sizeof(projectorDropDown)/sizeof(projectorDropDown[0]); i++) {
                    if(y > clickedCoord.y + (20 * i) && y <= clickedCoord.y + 20 + (20 * i)) {
                        projectorDropDownHoverIndex = i;
                        break;
                    }
                }
            }
        }
        else {
            gridDropDownHoverIndex = -1;
            projectorDropDownHoverIndex = -1;
        }
    }
    
    switch(headerHoverIndex) {
        case 0 : {
            if(x > 250 && x <= 250 + 144) {
                for(int i=0; i<sizeof(subFileHover)/sizeof(subFileHover[0]); i++) {
                    subFileHoverIndex = -1;
                    if(y > 25 + (20 * i) && y <= 25 + 20 + (20 * i)) {
                        subFileHoverIndex = i;
                        break;
                    }
                }
            }
            else
                subFileHoverIndex = -1;
            break;
        }
        case 1 : {
            if(x > 250 + 60 && x <= 250 + 60 + 144) {
                for(int i=0; i<sizeof(subModelingHover)/sizeof(subModelingHover[0]); i++) {
                    subModelingHoverIndex = -1;
                    if(y > 25 + (20 * i) && y <= 25 + 20 + (20 * i)) {
                        subModelingHoverIndex = i;
                        break;
                    }
                }
            }
            else
                subModelingHoverIndex = -1;
            break;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(currentSelectedProjector != -1 && button == OF_MOUSE_BUTTON_LEFT) {
        if(x >= 90 && x <= 90 + 450) {
            if(panelClickIndex == 0) {
                float realValue = (float)(x - 90) / 450 * 6000 - 3000;
                projectors[currentSelectedProjector]->xPos = realValue;
            }
            else if(panelClickIndex == 1) {
                float realValue = (float)(x - 90) / 450 * 6000 - 3000;
                projectors[currentSelectedProjector]->yPos = realValue;
            }
            else if(panelClickIndex == 2) {
                float realValue = (float)(x - 90) / 450 * 6000 - 3000;
                projectors[currentSelectedProjector]->zPos = realValue;
            }
            else if(panelClickIndex == 3) {
                float realValue = (float)(x - 90) / 450 * 720 - 360;
                projectors[currentSelectedProjector]->xRotation = realValue;
            }
            else if(panelClickIndex == 4) {
                float realValue = (float)(x - 90) / 450 * 720 - 360;
                projectors[currentSelectedProjector]->yRotation = realValue;
            }
            else if(panelClickIndex == 5) {
                float realValue = (float)(x - 90) / 450 * 720 - 360;
                projectors[currentSelectedProjector]->zRotation = realValue;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if(button == OF_MOUSE_BUTTON_LEFT) {
        if(isGridDropDownOn) {
            switch(gridDropDownHoverIndex) {
                case 0 : {
                    ofVec3f pt = findNearGridPoint(ofPoint(x,y));
                    addProjector(pt.x, pt.y, pt.z);
                    break;
                }
                case 1 : {
                    break;
                }
                case 2 : {
                    break;
                }
            }
            gridDropDownHoverIndex = -1;
            isGridDropDownOn = false;
        }
        else if(isProjectorDropDownOn) {
            switch(projectorDropDownHoverIndex) {
                case 0 : {
                    if(currentSelectedProjector != -1) {
                        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select Video file");
                        
                        if(openFileResult.bSuccess)
                            openVideoFile(openFileResult);
                    }
                    break;
                }
                case 1 : {
                    if(currentSelectedProjector != -1)
                        deleteProjector(currentSelectedProjector);
                    break;
                }
            }
            projectorDropDownHoverIndex = -1;
            isProjectorDropDownOn = false;
        }
        else {
            if(x > 50 && x <= 50 + 70) {
                if(y > 100 && y <= 100 + 70)
                    cameraButtonPressedIndex = 0;
                else if(y > 100 + 70 && y <= 100 + 70 + 70)
                    cameraButtonPressedIndex = 1;
                else
                    cameraButtonPressedIndex = -1;
            }
            else if(y > 145 && y <= 145 + 50) {
                if(x > 125 && x <= 125 + 50)
                    cameraButtonPressedIndex = 2;
                else if(x > 175 && x <= 175 + 50)
                    cameraButtonPressedIndex = 3;
                else
                    cameraButtonPressedIndex = -1;
            }
            else if(x > 150 && x < 150 + 50) {
                if(y > 100 && y <= 100 + 50)
                    cameraButtonPressedIndex = 4;
                else if(y > 190 && y <= 190 + 50)
                    cameraButtonPressedIndex = 5;
                else
                    cameraButtonPressedIndex = -1;
            }
            else
                cameraButtonPressedIndex = -1;
        }
        
        if(currentSelectedProjector != -1 && panelClickIndex == -1) {
            if(y > 590 && y <= 600) {
                float xPos = projectors[currentSelectedProjector]->xPos;
                float lengthRatio = (xPos + 3000) / 6000;
                float barLength = 450 * lengthRatio;
                if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                    panelClickIndex = 0;
                    easyCam.disableMouseInput();
                }
            }
            else if(y > 620 && y <= 630) {
                float yPos = projectors[currentSelectedProjector]->yPos;
                float lengthRatio = (yPos + 3000) / 6000;
                float barLength = 450 * lengthRatio;
                if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                    panelClickIndex = 1;
                    easyCam.disableMouseInput();
                }
            }
            else if(y > 650 && y <= 660) {
                float zPos = projectors[currentSelectedProjector]->zPos;
                float lengthRatio = (zPos + 3000) / 6000;
                float barLength = 450 * lengthRatio;
                if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                    panelClickIndex = 2;
                    easyCam.disableMouseInput();
                }
            }
            else if(y > 680 && y <= 690) {
                float xRotation = projectors[currentSelectedProjector]->xRotation;
                float lengthRatio = (xRotation + 360) / 720;
                float barLength = 450 * lengthRatio;
                if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                    panelClickIndex = 3;
                    easyCam.disableMouseInput();
                }
            }
            else if(y > 710 && y <= 720) {
                float yRotation = projectors[currentSelectedProjector]->yRotation;
                float lengthRatio = (yRotation + 360) / 720;
                float barLength = 450 * lengthRatio;
                if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                    panelClickIndex = 4;
                    easyCam.disableMouseInput();
                }
            }
            else if(y > 740 && y <= 750) {
                float zRotation = projectors[currentSelectedProjector]->zRotation;
                float lengthRatio = (zRotation + 360) / 720;
                float barLength = 450 * lengthRatio;
                if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                    panelClickIndex = 5;
                    easyCam.disableMouseInput();
                }
            }
        }
        
        if(y > ofGetHeight() - 220 && y <= ofGetHeight() - 220 + 20) {
            int index = 0;
            for(int i=0; i<7; i++) {
                if(x > 10 + (90 * i) && x <= 10 + (90 * (i + 1))) {
                    index = i;
                    break;
                }
            }
            int currentIndex = 0;
            for(int i=0; i<7; i++) {
                if(projectors[i]->isSetted) {
                    if(currentIndex == index) {
                        if(currentSelectedProjector != -1 && currentSelectedProjector != i)
                            projectors[currentSelectedProjector]->isSelected = false;
                        currentSelectedProjector = i;
                        projectors[currentSelectedProjector]->isSelected = true;
                        mappingGUI->setProjector(projectors[currentSelectedProjector]);
                        break;
                    }
                    else
                        currentIndex++;
                }
            }
        }
        
        
        if(y > 0 && y <= 25) {
            for(int i = 0; i < sizeof(headerHover)/sizeof(headerHover[0]); i++) {
                headerHoverIndex = -1;
                if(x > 250 + (60 * i) && x <= 250 + 60 + (60 * i)) {
                    headerHoverIndex = i;
                    break;
                }
            }
        }
        else
            headerHoverIndex = -1;
        
        switch(subFileHoverIndex) {
            case 0 : {
                ofLog() << "menu 1";
                break;
            }
            case 1 : {
                //ofLog() << "menu 2";
                save();
                break;
            }
            case 2 : {
                //ofLog() << "menu 3";
                load();
                break;
            }
                subFileHoverIndex = -1;
        }
        
        switch(subModelingHoverIndex) {
            case 0 : {
                //ofLog() << "menu 1";
                ofFileDialogResult openFileResult = ofSystemLoadDialog("Select 3d file");
                if(openFileResult.bSuccess) {
                    open3DFile(openFileResult);
                    subModelingHoverIndex = -1;
                }
                break;
            }
        }
        
        int nearestIndex = findNearProjectorIndex(ofVec2f(x,y));
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
                
                //if(projectors[currentSelectedProjector]->videoPlayer.isLoaded())
                mappingGUI->setProjector(projectors[nearestIndex]);
            }
            //refreshGUI();
        }
    }
    else if(button == OF_MOUSE_BUTTON_RIGHT) {
        clickedCoord = ofPoint(x,y);
        int nearestIndex = findNearProjectorIndex(clickedCoord);
        if(nearestIndex != -1) {
            isGridDropDownOn = false;
            if(currentSelectedProjector != nearestIndex) {
                if(currentSelectedProjector != -1)
                    projectors[currentSelectedProjector]->isSelected = false;
                projectors[nearestIndex]->isSelected = true;
                currentSelectedProjector = nearestIndex;
            }
            isProjectorDropDownOn = true;
        }
        else {
            isProjectorDropDownOn = false;
            isGridDropDownOn = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if(cameraButtonPressedIndex != -1)
        cameraButtonPressedIndex = -1;
    if(panelClickIndex != -1)
        panelClickIndex = -1;
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

void ofApp::addProjector(float x, float y, float z) {
    for(int i=0; i<projectors.size(); i++) {
        if(!projectors[i]->isSetted) {
            if(currentSelectedProjector != -1)
                projectors[currentSelectedProjector]->isSelected = false;
            projectors[i]->activate(x, y, z);
            currentSelectedProjector = i;
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
        for(int j=0; j<meshes.size(); j++)
            meshes[j].draw();
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
    
    for(int j=0; j<meshes.size(); j++)
        meshes[j].draw();
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
        
        reconstructMesh();
    }
}

void ofApp::open3DFile(string url) {
    //for the save, not implemented yet!!!
}

void ofApp::reconstructMesh() {
    if(!meshes.empty())
        meshes.clear();
    for(int i = 0; i < model.getMeshCount(); i++) {
        meshes.push_back(model.getMesh(i));
        ofMatrix4x4 modelMatrix = model.getModelMatrix();
        ofMatrix4x4 meshMatrix = model.getMeshHelper(i).matrix;
        ofMatrix4x4 concatMatrix;
        concatMatrix.preMult(modelMatrix);
        concatMatrix.preMult(meshMatrix);
        
        //Reconstruct mesh's vertices and normals from the model object
        for(int j=0; j<meshes[i].getNumVertices(); j++) {
            ofVec3f& vert = meshes[i].getVertices()[j];
            vert.set(concatMatrix.preMult(vert));
            ofVec3f& norm = meshes[i].getNormals()[j];
            norm.set(0, 0, 0);
        }
        
        //Reconstruct normal vector
        for(int j = 0; j < meshes[i].getIndices().size(); j += 3) {
            int aIndex = meshes[i].getIndices()[j];
            int bIndex = meshes[i].getIndices()[j + 1];
            int cIndex = meshes[i].getIndices()[j + 2];
            
            ofVec3f offset1 = meshes[i].getVertices()[aIndex] - meshes[i].getVertices()[bIndex];
            ofVec3f offset2 = meshes[i].getVertices()[cIndex] - meshes[i].getVertices()[bIndex];
            ofVec3f normalValue = offset2.cross(offset1);
            
            meshes[i].getNormals()[aIndex] += normalValue;
            meshes[i].getNormals()[bIndex] += normalValue;
            meshes[i].getNormals()[cIndex] += normalValue;
        }
    }
    
    //Find lowest y value and closest z value then, reset position
    float leftestX = 3000;
    float lowestY = 3000;
    float closestZ = -3000;
    
    for(int i = 0; i < model.getMeshCount(); i++) {
        for(int j=0; j<meshes[i].getNumVertices(); j++) {
            ofVec3f& vert = meshes[i].getVertices()[j];
            
            if(vert.x < leftestX)
                leftestX = vert.x;
            if(vert.y < lowestY)
                lowestY = vert.y;
            if(vert.z > closestZ)
                closestZ = vert.z;
        }
    }
    
    model.setPosition(-leftestX,-lowestY,-closestZ);
    //model.setPosition(0, -lowestY, -closestZ);
    
    for(int i = 0; i < model.getMeshCount(); i++) {
        //DO AGAIN
        ofMatrix4x4 modelMatrix = model.getModelMatrix();
        ofMatrix4x4 meshMatrix = model.getMeshHelper(i).matrix;
        ofMatrix4x4 concatMatrix2;
        concatMatrix2.preMult(modelMatrix);
        concatMatrix2.preMult(meshMatrix);
        for(int j = 0; j < meshes[i].getNumVertices(); j++) {
            //translate vertices and normals
            ofVec3f& vert = meshes[i].getVertices()[j];
            vert += ofVec3f(-leftestX, -lowestY, -closestZ);
            //vert += ofVec3f(0, -lowestY, -closestZ);
            ofVec3f& norm = meshes[i].getNormals()[j];
            norm += ofVec3f(-leftestX, -lowestY, -closestZ);
            //norm += ofVec3f(0, -lowestY, -closestZ);
        }
    }
}

void ofApp::reconstructProjectorMesh() {
    projectorMesh = projectorModel.getMesh(0);
    
    ofMatrix4x4 modelMatrix = projectorModel.getModelMatrix();
    ofMatrix4x4 meshMatrix = projectorModel.getMeshHelper(0).matrix;
    ofMatrix4x4 concatMatrix;
    concatMatrix.preMult(modelMatrix);
    concatMatrix.preMult(meshMatrix);
    
    //Reconstruct mesh's vertices and normals from the model object
    for(int i=0; i<projectorMesh.getNumVertices(); i++) {
        ofVec3f& vert = projectorMesh.getVertices()[i];
        vert.set(concatMatrix.preMult(vert));
        ofVec3f& norm = projectorMesh.getNormals()[i];
        norm.set(0, 0, 0);
    }
    
    //Reconstruct normal vector
    for(int i = 0; i < projectorMesh.getIndices().size(); i += 3) {
        int aIndex = projectorMesh.getIndices()[i];
        int bIndex = projectorMesh.getIndices()[i + 1];
        int cIndex = projectorMesh.getIndices()[i + 2];
        
        ofVec3f offset1 = projectorMesh.getVertices()[aIndex] - projectorMesh.getVertices()[bIndex];
        ofVec3f offset2 = projectorMesh.getVertices()[cIndex] - projectorMesh.getVertices()[bIndex];
        ofVec3f normalValue = offset2.cross(offset1);
        
        projectorMesh.getNormals()[aIndex] += normalValue;
        projectorMesh.getNormals()[bIndex] += normalValue;
        projectorMesh.getNormals()[cIndex] += normalValue;
    }
}

void ofApp::openVideoFile(ofFileDialogResult openFileResult) {
    ofFile file(openFileResult.getPath());
    
    if(file.exists()) {
        string fileExtension = ofToUpper(file.getExtension());
        
        if(fileExtension == "MOV" || fileExtension == "MP4" || fileExtension == "AVI") {
            projectors[currentSelectedProjector]->videoPlayer.load(openFileResult.getPath());
            projectors[currentSelectedProjector]->videoPlayer.play();
            projectors[currentSelectedProjector]->currentVideoURL = openFileResult.getPath();
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

void ofApp::drawDropDownMenus() {
    if(isGridDropDownOn) {
        for(int i = 0; i < sizeof(gridDropDown)/sizeof(gridDropDown[0]); i++) {
            if(gridDropDownHoverIndex==i)
                gridDropDownHover[i].draw(clickedCoord.x, clickedCoord.y + (20 * i), 144, 20);
            else
                gridDropDown[i].draw(clickedCoord.x, clickedCoord.y + (20 * i), 144, 20);
        }
    }
    else if(isProjectorDropDownOn) {
        for(int i = 0; i < sizeof(projectorDropDown)/sizeof(projectorDropDown[0]); i++) {
            if(projectorDropDownHoverIndex == i)
                projectorDropDownHover[i].draw(clickedCoord.x, clickedCoord.y + (20 * i), 144, 20);
            else
                projectorDropDown[i].draw(clickedCoord.x, clickedCoord.y + (20 * i), 144, 20);
        }
    }
}

void ofApp::drawButtons() {
    for(int i=0; i<2; i++) {
        if(cameraButtonPressedIndex == i)
            cameraButtonPressed[i].draw(50, 100 + (70 * i), 70, 70);
        else
            cameraButton[i].draw(50, 100 + (70 * i), 70, 70);
    }
    
    if(cameraButtonPressedIndex == 2)
        cameraButtonPressed[2].draw(125, 145, 50, 50);
    else
        cameraButton[2].draw(125, 145, 50, 50);
    
    if(cameraButtonPressedIndex == 3)
        cameraButtonPressed[3].draw(175, 145, 50, 50);
    else
        cameraButton[3].draw(175, 145, 50, 50);
    
    if(cameraButtonPressedIndex == 4)
        cameraButtonPressed[4].draw(150, 100, 50, 50);
    else
        cameraButton[4].draw(150, 100, 50, 50);
    
    if(cameraButtonPressedIndex == 5)
        cameraButtonPressed[5].draw(150, 190, 50, 50);
    else
        cameraButton[5].draw(150, 190, 50, 50);
}

void ofApp::drawHeaders() {
    for(int i = 0; i < sizeof(headerMenu)/sizeof(headerMenu[0]); i++) {
        if(headerHoverIndex == i)
            headerHover[i].draw(250 + (60 * i), 0, 60, 25);
        else
            headerMenu[i].draw(250 + (60 * i), 0, 60, 25);
    }
    
    switch(headerHoverIndex) {
        case 0 : {
            for(int i = 0; i < sizeof(subMenuFile)/sizeof(subMenuFile[0]); i++) {
                if(subFileHoverIndex==i)
                    subFileHover[i].draw(250, 25 + (20 * i), 144, 20);
                else
                    subMenuFile[i].draw(250, 25 + (20 * i), 144, 20);
            }
            break;
        }
        case 1 : {
            for(int i = 0; i < sizeof(subMenuModeling)/sizeof(subMenuModeling[0]); i++) {
                if(subModelingHoverIndex==i)
                    subModelingHover[i].draw(250 + 60, 25 + (20 * i), 144, 20);
                else
                    subMenuModeling[i].draw(250 + 60, 25 + (20 * i), 144, 20);
            }
            break;
        }
    }
}

void ofApp::drawPanel() {
    if(currentSelectedProjector != -1) {
        int currentIndex = 0;
        for(int i=0; i<7; i++) {
            if(projectors[i]->isSetted) {
                if(projectors[i]->isSelected) {
                    panelLabel.draw(10 + (90 * currentIndex), ofGetHeight() - 220, 90, 20);
                }
                else {
                    panelLabelClick.draw(10 + (90 * currentIndex), ofGetHeight() - 220, 90, 20);
                }
                ofDrawBitmapString("Proj. " + ofToString(i), 25 + (90 * currentIndex), ofGetHeight() - 220 + 15);
                currentIndex++;
            }
        }
        panelWindow.draw(10, ofGetHeight() - 200, 600, 190);
        
        float xPos = projectors[currentSelectedProjector]->xPos;
        float yPos = projectors[currentSelectedProjector]->yPos;
        float zPos = projectors[currentSelectedProjector]->zPos;
        float xRotation = projectors[currentSelectedProjector]->xRotation;
        float yRotation = projectors[currentSelectedProjector]->yRotation;
        float zRotation = projectors[currentSelectedProjector]->zRotation;
        
        ofDrawBitmapString("X Pos", 20, 600);
        ofDrawBitmapString("Y Pos", 20, 630);
        ofDrawBitmapString("Z Pos", 20, 660);
        ofDrawBitmapString("X Rotate", 20, 690);
        ofDrawBitmapString("Y Rotate", 20, 720);
        ofDrawBitmapString("Z Rotate", 20, 750);
        
        ofDrawBitmapString(ofToString(xPos), 550, 600);
        ofDrawBitmapString(ofToString(yPos), 550, 630);
        ofDrawBitmapString(ofToString(zPos), 550, 660);
        ofDrawBitmapString(ofToString(xRotation), 550, 690);
        ofDrawBitmapString(ofToString(yRotation), 550, 720);
        ofDrawBitmapString(ofToString(zRotation), 550, 750);
        
        panelGrayBar.draw(90, 590, 450, 10);
        panelGrayBar.draw(90, 620, 450, 10);
        panelGrayBar.draw(90, 650, 450, 10);
        panelGrayBar.draw(90, 680, 450, 10);
        panelGrayBar.draw(90, 710, 450, 10);
        panelGrayBar.draw(90, 740, 450, 10);
        
        float lengthRatio = (xPos + 3000) / 6000;
        float barLength = 450 * lengthRatio;
        panelGreenBar.draw(90, 590, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 585, 20, 20);
        lengthRatio = (yPos + 3000) / 6000;
        barLength = 450 * lengthRatio;
        panelGreenBar.draw(90, 620, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 615, 20, 20);
        lengthRatio = (zPos + 3000) / 6000;
        barLength = 450 * lengthRatio;
        panelGreenBar.draw(90, 650, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 645, 20, 20);
        lengthRatio = (xRotation + 360) / 720;
        barLength = 450 * lengthRatio;
        panelGreenBar.draw(90, 680, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 675, 20, 20);
        lengthRatio = (yRotation + 360) / 720;
        barLength = 450 * lengthRatio;
        panelGreenBar.draw(90, 710, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 705, 20, 20);
        lengthRatio = (zRotation + 360) / 720;
        barLength = 450 * lengthRatio;
        panelGreenBar.draw(90, 740, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 735, 20, 20);
        
        panelWindow.draw(620, ofGetHeight() - 200, 300, 190);
        string str = "current selected projector : ";
        str += ofToString(currentSelectedProjector);
        str += "\n";
        
        str += "offset from modeling : ";
        str += ofToString(projectors[currentSelectedProjector]->zPos/100);
        str += "M\n";
        
        if(projectors[currentSelectedProjector]->xPos < 0) {
            str += "left offset from modeling : ";
            str += ofToString(projectors[currentSelectedProjector]->xPos/100 * -1);
            str += "M\n";
        }
        else if(projectors[currentSelectedProjector]->xPos > 0) {
            str += "right offset from modeling : ";
            str += ofToString(projectors[currentSelectedProjector]->xPos/100);
            str += "M\n";
        }
        
        str += "upper offset from ground : ";
        str += ofToString(projectors[currentSelectedProjector]->yPos/100);
        str += "M\n";
        ofDrawBitmapString(str, 650, 625);
    }
}

void ofApp::save() {
    ofFile textFile;
    ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + ".txt", "Save your file");
    
    if(saveFileResult.bSuccess) {
        textFile.open(saveFileResult.getPath(), ofFile::WriteOnly);
        if(currentModelURL.compare("")==0)
            textFile << "no modeling" << endl;
        else
            textFile << currentModelURL << endl;
        for(int i=0; i<projectors.size(); i++) {
            if(projectors[i]->isSetted) {
                textFile << projectors[i]->xPos << endl;
                textFile << projectors[i]->yPos << endl;
                textFile << projectors[i]->zPos << endl;
                textFile << projectors[i]->xRotation << endl;
                textFile << projectors[i]->yRotation << endl;
                textFile << projectors[i]->zRotation << endl;
                textFile << projectors[i]->pt[0].x << endl;
                textFile << projectors[i]->pt[0].y << endl;
                textFile << projectors[i]->pt[1].x << endl;
                textFile << projectors[i]->pt[1].y << endl;
                textFile << projectors[i]->pt[2].x << endl;
                textFile << projectors[i]->pt[2].y << endl;
                textFile << projectors[i]->pt[3].x << endl;
                textFile << projectors[i]->pt[3].y << endl;
                if(!projectors[i]->isMappingOn) {
                    textFile << "not mapped" << endl;
                }
                else {
                    textFile << "mapped" << endl;
                }
                if(projectors[i]->videoPlayer.isLoaded()) {
                    textFile << projectors[i]->currentVideoURL << endl;
                }
                else {
                    textFile << "no video" << endl;
                }
            }
            else {
                textFile << "not setted" << endl;
            }
        }
        textFile.close();
    }
}

void ofApp::load() {
    ofFile textFile;
    ofBuffer buffer;
    int currentLine = 0;
    string str;
    ofFileDialogResult loadFileResult = ofSystemLoadDialog("load file");
    if(loadFileResult.bSuccess) {
        //open3DFile(openFileResult);
        textFile.open(loadFileResult.getPath(), ofFile::ReadOnly);
        buffer = textFile.readToBuffer();
        str = ofSplitString(buffer.getText(), "\n")[currentLine];
        
        if(str.compare("no modeling") != 0)
            open3DFile(str);
        currentLine++;
        for(int i=0; i<projectors.size(); i++) {
            str = ofSplitString(buffer.getText(), "\n")[currentLine];
            if(str.compare("not setted") != 0) {
                projectors[i]->isSetted = true;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->xPos = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->yPos = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->zPos = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->xRotation = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->yRotation = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->zRotation = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->pt[0].x = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->pt[0].y = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->pt[1].x = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->pt[1].y = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->pt[2].x = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->pt[2].y = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->pt[3].x = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                projectors[i]->pt[3].y = stof(str);
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                if(str.compare("not mapped")!=0)
                    projectors[i]->isMappingOn = true;
                currentLine++;
                
                str = ofSplitString(buffer.getText(), "\n")[currentLine];
                if(str.compare("no video")!=0) {
                    projectors[i]->currentVideoURL = str;
                    projectors[i]->videoPlayer.load(str);
                    projectors[i]->videoPlayer.play();
                }
            }
            currentLine++;
        }
    }
}

ofVec3f ofApp::findNearGridPoint(ofPoint mousePt) {
    float nearestDistance = 50;
    ofVec3f nearestPoint = ofVec3f(0,0,0);
    for(int i=-3000; i<=3000; i+=200) {
        for(int j=-3000; j<=3000; j+=200) {
            ofVec3f gridPt = easyCam.worldToScreen(ofVec3f(i, 0, j));
            float distance = gridPt.distance(mousePt);
            if(distance < 50 && distance < nearestDistance) {
                nearestDistance = distance;
                nearestPoint = ofVec3f(i,100,j);
            }
        }
    }
    return nearestPoint;
}

int ofApp::findNearProjectorIndex(ofPoint mousePt) {
    float nearestDistance = 50;
    int nearestIndex = -1;
    for(int i=0; i<projectors.size(); i++) {
        ofVec3f projectorPt = easyCam.worldToScreen(ofVec3f(projectors[i]->xPos, projectors[i]->yPos, projectors[i]->zPos));
        float distance = projectorPt.distance(mousePt);
        if(distance < 50 && distance < nearestDistance) {
            nearestDistance = distance;
            if(projectors[i]->isSetted)
                nearestIndex = i;
        }
    }
    return nearestIndex;
}
