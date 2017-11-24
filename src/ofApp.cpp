#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(70,70,70);
    ofEnableDepthTest(); //draw in order of z value
    
    //allocating new projector class - 3 projectors only due to the performance issue
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    //projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    //projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    //projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    //projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    //projectors.push_back(new Projector(300,20,850,0,0,0,projectors.size()));
    //projectors.push_back(new Projector(400,20,850,0,0,0,projectors.size()));
    
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->setup();
    
    //camera set
    easyCam.setFarClip(12000);
    
    //load shaders
    textureProjectionShader.load("TextureProjection");
    depthStoringShader.load("DepthStoring");
    projectorTextureShader.load("ProjectorTexture");
    
    currentSelectedProjector = -1;
    isModelingSelected = -1;
    //gui.setup();
    
    //UI resource setup
    gridDropDown[0].load("UI/MouseDropDown/makeProjector.png");
    gridDropDownHover[0].load("UI/MouseDropDown/makeProjectorHover.png");
    gridDropDownHoverIndex = -1;
    isGridDropDownOn = false;
    
    projectorDropDown[0].load("UI/MouseDropDown/videoLoad.png");
    projectorDropDown[1].load("UI/MouseDropDown/deleteProjector.png");
    projectorDropDownHover[0].load("UI/MouseDropDown/videoLoadHover.png");
    projectorDropDownHover[1].load("UI/MouseDropDown/deleteProjectorHover.png");
    projectorDropDownHoverIndex = -1;
    isProjectorDropDownOn = false;
    
    headerMenu[0].load("UI/UpperMenu/file.png");
    headerMenu[1].load("UI/UpperMenu/modeling.png");
    headerHover[0].load("UI/UpperMenu/fileClick.png");
    headerHover[1].load("UI/UpperMenu/modelingClick.png");
    headerHoverIndex = -1;
    
    subMenuFile[0].load("UI/UpperMenu/newProject.png");
    subMenuFile[1].load("UI/UpperMenu/saveProject.png");
    subMenuFile[2].load("UI/UpperMenu/loadProject.png");
    subFileHover[0].load("UI/UpperMenu/newProjectHover.png");
    subFileHover[1].load("UI/UpperMenu/saveProjectHover.png");
    subFileHover[2].load("UI/UpperMenu/loadProjectHover.png");
    subFileHoverIndex = -1;
    
    subMenuModeling[0].load("UI/UpperMenu/loadModeling.png");
    subModelingHover[0].load("UI/UpperMenu/loadModelingHover.png");
    subModelingHoverIndex = -1;
    
    cameraZoomBackground.load("UI/Button/+-Background.png");
    cameraMoveBackground.load("UI/Button/udlrBackground.png");
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
    
    //model.loadModel("projector.3ds");
    //model.setPosition(0, 0, 0);
    //model.setScale(1, 1, 1);
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
        //easyCam.move(-10, 0, 0);
        //easyCam.setTarget(ofVec3f(easyCam.getTarget().getX() - 10,easyCam.getTarget().getY(), easyCam.getTarget().getZ()));
        easyCam.move(easyCam.getSideDir() * -10);
        float x = easyCam.getTarget().getX() - easyCam.getSideDir().x * 10;
        float y = easyCam.getTarget().getY() - easyCam.getSideDir().y * 10;
        float z = easyCam.getTarget().getZ() - easyCam.getSideDir().z * 10;
        easyCam.setTarget(ofVec3f(x,y,z));
    }
    else if(cameraButtonPressedIndex==3) {
        easyCam.disableMouseInput();
        //easyCam.move(10, 0, 0);
        //easyCam.setTarget(ofVec3f(easyCam.getTarget().getX() + 10,easyCam.getTarget().getY(), easyCam.getTarget().getZ()));
        easyCam.move(easyCam.getSideDir() * 10);
        float x = easyCam.getTarget().getX() + easyCam.getSideDir().x * 10;
        float y = easyCam.getTarget().getY() + easyCam.getSideDir().y * 10;
        float z = easyCam.getTarget().getZ() + easyCam.getSideDir().z * 10;
        easyCam.setTarget(ofVec3f(x, y, z));
    }
    else if(cameraButtonPressedIndex==4) {
        easyCam.disableMouseInput();
        //easyCam.move(0, 10, 0);
        //easyCam.setTarget(ofVec3f(easyCam.getTarget().getX(), easyCam.getTarget().getY() + 10, easyCam.getTarget().getZ()));
        easyCam.move(easyCam.getUpDir() * 10);
        float x = easyCam.getTarget().getX() + easyCam.getUpDir().x * 10;
        float y = easyCam.getTarget().getY() + easyCam.getUpDir().y * 10;
        float z = easyCam.getTarget().getZ() + easyCam.getUpDir().z * 10;
        easyCam.setTarget(ofVec3f(x, y, z));
    }
    else if(cameraButtonPressedIndex==5) {
        easyCam.disableMouseInput();
        //easyCam.move(0, -10, 0);
        //easyCam.setTarget(ofVec3f(easyCam.getTarget().getX(), easyCam.getTarget().getY() - 10, easyCam.getTarget().getZ()));
        easyCam.move(easyCam.getUpDir() * -10);
        float x = easyCam.getTarget().getX() - easyCam.getUpDir().x * 10;
        float y = easyCam.getTarget().getY() - easyCam.getUpDir().y * 10;
        float z = easyCam.getTarget().getZ() - easyCam.getUpDir().z * 10;
        easyCam.setTarget(ofVec3f(x, y, z));
    }
    else {
        if(!easyCam.getMouseInputEnabled()) {
            easyCam.enableMouseInput();
            if(panelClickIndex == -1) {
                ofNode t = easyCam.getTarget();
                ofVec3f p = easyCam.getPosition();
                //easyCam.enableMouseInput();
                easyCam.reset();
                easyCam.setPosition(p.x, p.y, p.z);
                easyCam.setTarget(t);
            }
        }
    }
    
    ofDisableDepthTest();
    ofDrawGrid(200, 15, true, false, true, false);
    ofEnableDepthTest();
    
    renderCustomModel();
    
    //model.drawFaces();
    /*reconstructMesh();
    for(int i=0; i<meshes.size(); i++) {
        meshes[i].draw();
    }*/
    
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
    drawButtons();
    ofEnableDepthTest();
    
    drawHeaders();
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
        /*case 0 : {
            float currentYPos = 20 + headerMenu[0].getHeight();
            if(x > ofGetWidth() - 200 && x <= ofGetWidth() - 200 + subMenuFile[0].getWidth()) {
                for(int i=0; i<sizeof(subFileHover)/sizeof(subFileHover[0]); i++) {
                    subFileHoverIndex = -1;
                    if(y > currentYPos && y <= currentYPos + subMenuFile[i].getHeight()) {
                        subFileHoverIndex = i;
                        break;
                    }
                    currentYPos += subMenuFile[i].getHeight();
                }
            }
            else
                subFileHoverIndex = -1;
            break;
        }
        case 1 : {
            float currentYPos = 20 + headerMenu[0].getHeight();
            if(x > ofGetWidth() - 200 && x <= ofGetWidth() - 200 + subMenuFile[0].getWidth()) {
                for(int i=0; i<sizeof(subModelingHover)/sizeof(subModelingHover[0]); i++) {
                    subModelingHoverIndex = -1;
                    if(y > currentYPos && y <= currentYPos + subMenuModeling[i].getHeight()) {
                        subModelingHoverIndex = i;
                        break;
                    }
                    currentYPos += subMenuModeling[i].getHeight();
                }
            }
            else
                subModelingHoverIndex = -1;
            break;
        }*/
        case 0 : {
            float currentYPos = 20 + headerMenu[0].getHeight()/4*3;
            if(x > ofGetWidth() - 200 && x <= ofGetWidth() - 200 + subMenuFile[0].getWidth()/4*3) {
                for(int i=0; i<sizeof(subFileHover)/sizeof(subFileHover[0]); i++) {
                    subFileHoverIndex = -1;
                    if(y > currentYPos && y <= currentYPos + subMenuFile[i].getHeight()/4*3) {
                        subFileHoverIndex = i;
                        break;
                    }
                    currentYPos += subMenuFile[i].getHeight()/4*3;
                }
            }
            else
                subFileHoverIndex = -1;
            break;
        }
        case 1 : {
            float currentYPos = 20 + headerMenu[0].getHeight()/4*3;
            if(x > ofGetWidth() - 200 && x <= ofGetWidth() - 200 + subMenuFile[0].getWidth()/4*3) {
                for(int i=0; i<sizeof(subModelingHover)/sizeof(subModelingHover[0]); i++) {
                    subModelingHoverIndex = -1;
                    if(y > currentYPos && y <= currentYPos + subMenuModeling[i].getHeight()/4*3) {
                        subModelingHoverIndex = i;
                        break;
                    }
                    currentYPos += subMenuModeling[i].getHeight()/4*3;
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
    else if(isModelingSelected == 1) {
        if(panelClickIndex == 6) {
            float realValue = (float)(x - 120) / 450 * 50;
            float offset = realValue - model.getScale().x;
            ofLog() << offset;
            scaleModeling(offset);
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
                        easyCam.disableMouseInput();
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
            /*if(y > ofGetHeight()/2 - 200 && y <= ofGetHeight()/2 - 200 + cameraZoomBackground.getHeight()) {
                if(x > 56 && x <= 56 + 56) {
                    if(y > ofGetHeight()/2 - 192 && y <= ofGetHeight()/2 - 192 + 72)
                        cameraButtonPressedIndex = 0;
                    else if(y > ofGetHeight()/2 - 192 + 72 && y <= ofGetHeight()/2 - 192 + (72 * 2))
                        cameraButtonPressedIndex = 1;
                    else
                        cameraButtonPressedIndex = -1;
                }
            }
            else if(y > ofGetHeight()/2 && y <= ofGetHeight()/2 + cameraMoveBackground.getHeight()) {
                if(y > ofGetHeight()/2 + 45 && y <= ofGetHeight()/2 + 45 + cameraButton[2].getHeight()) {
                    if(x > 28 && x <= 28 + cameraButton[2].getWidth())
                        cameraButtonPressedIndex = 2;
                    else if(x > 106 && x <= 106 + cameraButton[2].getWidth())
                        cameraButtonPressedIndex = 3;
                    else
                        cameraButtonPressedIndex = -1;
                }
                else if(x > 67 && x <= 67 + cameraButton[2].getWidth()) {
                    if(y > ofGetHeight()/2 + 5 && y <= ofGetHeight()/2 + 5 + cameraButton[2].getHeight())
                        cameraButtonPressedIndex = 4;
                    else if(y > ofGetHeight()/2 + 81 && y <= ofGetHeight()/2 + 81 + cameraButton[2].getHeight())
                        cameraButtonPressedIndex = 5;
                    else
                        cameraButtonPressedIndex = -1;
                }
                else
                    cameraButtonPressedIndex = -1;
            }*/
            
            //3/4 version
            if(y > (ofGetHeight()/2 - 200)/4*3 && y <= (ofGetHeight()/2 - 200 + cameraZoomBackground.getHeight())/4*3) {
                if(x > 56/4*3 && x <= (56 + 56)/4*3) {
                    if(y > (ofGetHeight()/2 - 192)/4*3 && y <= (ofGetHeight()/2 - 192 + 72)/4*3)
                        cameraButtonPressedIndex = 0;
                    else if(y > (ofGetHeight()/2 - 192 + 72)/4*3 && y <= (ofGetHeight()/2 - 192 + (72 * 2))/4*3)
                        cameraButtonPressedIndex = 1;
                    else
                        cameraButtonPressedIndex = -1;
                }
            }
            else if(y > ofGetHeight()/2/4*3 && y <= (ofGetHeight()/2 + cameraMoveBackground.getHeight())/4*3) {
                if(y > (ofGetHeight()/2 + 45)/4*3 && y <= (ofGetHeight()/2 + 45 + cameraButton[2].getHeight())/4*3) {
                    if(x > 28/4*3 && x <= (28 + cameraButton[2].getWidth())/4*3) {
                        cameraButtonPressedIndex = 2;
                        model.setScale(model.getScale().x + 0.01, model.getScale().x + 0.01, model.getScale().x + 0.01);
                        reconstructMesh();
                    }
                    else if(x > 106/4*3 && x <= (106 + cameraButton[2].getWidth())/4*3)
                        cameraButtonPressedIndex = 3;
                    else
                        cameraButtonPressedIndex = -1;
                }
                else if(x > 67/4*3 && x <= (67 + cameraButton[2].getWidth())/4*3) {
                    if(y > (ofGetHeight()/2 + 5)/4*3 && y <= (ofGetHeight()/2 + 5 + cameraButton[2].getHeight())/4*3)
                        cameraButtonPressedIndex = 4;
                    else if(y > (ofGetHeight()/2 + 81)/4*3 && y <= (ofGetHeight()/2 + 81 + cameraButton[2].getHeight())/4*3)
                        cameraButtonPressedIndex = 5;
                    else
                        cameraButtonPressedIndex = -1;
                }
                else
                    cameraButtonPressedIndex = -1;
            }
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
        else if(isModelingSelected == 1) {
            if(y > ofGetHeight() - 95 && y <= ofGetHeight() - 85) {
                float currentScale = model.getScale().x;
                float lengthRatio = currentScale / 50;
                float barLength = 450 * lengthRatio;
                if(x > 120 + barLength - 10 && x <= 120 + barLength + 10) {
                    panelClickIndex = 6;
                    easyCam.disableMouseInput();
                }
            }
        }
        
        if(y > ofGetHeight() - 200 && y <= ofGetHeight() - 200 + 18) {
            int index = 0;
            for(int i=0; i<7; i++) {
                if(x > 10 + (90 * i) && x <= 10 + (90 * (i + 1))) {
                    index = i;
                    break;
                }
            }
            int currentIndex = 0;
            for(int i=0; i<projectors.size(); i++) {
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
        
        
        /*if(y > 20 && y <= 20 + headerMenu[0].getHeight()) {
            for(int i = 0; i < sizeof(headerHover)/sizeof(headerHover[0]); i++) {
                headerHoverIndex = -1;
                if(i != 0) {
                    if(x > ofGetWidth() - 200 + headerMenu[i-1].getWidth() && x <= ofGetWidth() - 200 + headerMenu[i-1].getWidth() + headerMenu[i].getWidth()) {
                        headerHoverIndex = i;
                        break;
                    }
                }
                else {
                    if(x > ofGetWidth() - 200 && x <= ofGetWidth() - 200 + headerMenu[i].getWidth()) {
                        headerHoverIndex = i;
                        break;
                    }
                }
            }
        }*/
        if(y > 20 && y <= 20 + (headerMenu[0].getHeight())/4*3) { //3/4 version
            for(int i = 0; i < sizeof(headerHover)/sizeof(headerHover[0]); i++) {
                headerHoverIndex = -1;
                if(i != 0) {
                    if(x > ofGetWidth() - 200 + (headerMenu[i-1].getWidth())/4*3 && x <= ofGetWidth() - 200 + (headerMenu[i-1].getWidth() + headerMenu[i].getWidth())/4*3) {
                        headerHoverIndex = i;
                        break;
                    }
                }
                else {
                    if(x > ofGetWidth() - 200 && x <= ofGetWidth() - 200 + (headerMenu[i].getWidth())/4*3) {
                        headerHoverIndex = i;
                        break;
                    }
                }
            }
        }
        else
            headerHoverIndex = -1;
        
        switch(subFileHoverIndex) {
            case 0 : {
                //ofLog() << "menu 1";
                break;
            }
            case 1 : {
                //ofLog() << "menu 2";
                easyCam.disableMouseInput();
                save();
                break;
            }
            case 2 : {
                //ofLog() << "menu 3";
                easyCam.disableMouseInput();
                load();
                break;
            }
            //subFileHoverIndex = -1;
        }
        subFileHoverIndex = -1;
        
        switch(subModelingHoverIndex) {
            case 0 : {
                //ofLog() << "menu 1";
                easyCam.disableMouseInput();
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
        else {
            if(isModelingSelected == 1) {
                if(currentSelectedProjector != -1)
                    projectors[currentSelectedProjector]->isSelected = false;
                mappingGUI->projector = NULL;
            }
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
                mappingGUI->setProjector(projectors[nearestIndex]);
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
            mappingGUI->setProjector(projectors[currentSelectedProjector]);
            break;
        }
    }
}

void ofApp::deleteProjector(int projectorNum) {
    projectors[projectorNum]->deactivate();
    currentSelectedProjector = -1;
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
    textureProjectionShader.setUniform1f("isModelingSelected", (float)isModelingSelected);
    //ofLog() << currentSelectedProjector;
    
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
           fileExtension == "COLLADA" || fileExtension == "DAE") {
            model.loadModel(openFileResult.getPath());
        }
        
        currentModelURL = openFileResult.getPath();
        
        model.setPosition(0, 0, 0);
        model.setScale(5, 5, 5);
        model.setRotation(0, 180, 0, 0, 1);
        
        reconstructMesh();
    }
}

void ofApp::open3DFile(string url) {
    model.loadModel(url);
    
    currentModelURL = url;
    
    model.setPosition(0, 0, 0);
    model.setScale(5, 5, 5);
    model.setRotation(0, 180, 0, 0, 1);
    
    reconstructMesh();
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
    float rightestX = -3000;
    float highestY = -3000;
    float lowestY = 3000;
    float closestZ = -3000;
    
    for(int i = 0; i < model.getMeshCount(); i++) {
        for(int j=0; j<meshes[i].getNumVertices(); j++) {
            ofVec3f& vert = meshes[i].getVertices()[j];
            
            if(vert.x < leftestX)
                leftestX = vert.x;
            if(vert.x > rightestX)
                rightestX = vert.x;
            if(vert.y > highestY)
                highestY = vert.y;
            if(vert.y < lowestY)
                lowestY = vert.y;
            if(vert.z > closestZ)
                closestZ = vert.z;
        }
    }
    
    modelWidth = abs(rightestX - leftestX);
    modelHeight = abs(highestY - lowestY);
    
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
    /*else if(index==3) {
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
    else if(index==7) {
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
    //cameraZoomBackground.draw(50, ofGetHeight()/2 - 200);
    cameraZoomBackground.draw(50/4*3, (ofGetHeight()/2 - 200)/4*3, cameraZoomBackground.getWidth()/4*3, cameraZoomBackground.getHeight()/4*3);
    
    for(int i=0; i<2; i++) {
        if(cameraButtonPressedIndex == i) {
            //cameraButtonPressed[i].draw(56, ofGetHeight()/2 - 192 + (72 * i), 56, 56);
            cameraButtonPressed[i].draw(56/4*3, (ofGetHeight()/2 - 192 + (72 * i))/4*3, 56/4*3, 56/4*3);
        }
        else {
            //cameraButton[i].draw(56, ofGetHeight()/2 - 192 + (72 * i), 56, 56);
            cameraButton[i].draw(56/4*3, (ofGetHeight()/2 - 192 + (72 * i))/4*3, 56/4*3, 56/4*3);
        }
    }
    
    //cameraMoveBackground.draw(23, ofGetHeight()/2);
    cameraMoveBackground.draw(23/4*3, (ofGetHeight()/2)/4*3, cameraMoveBackground.getWidth()/4*3, cameraMoveBackground.getHeight()/4*3);
    
    if(cameraButtonPressedIndex == 2) {//LEFT
        //cameraButtonPressed[2].draw(28, ofGetHeight()/2 + 45);
        cameraButtonPressed[2].draw(28/4*3, (ofGetHeight()/2 + 45)/4*3, cameraButtonPressed[2].getWidth()/4*3, cameraButtonPressed[2].getHeight()/4*3);
    }
    else {
        //cameraButton[2].draw(28, ofGetHeight()/2 + 45);
        cameraButton[2].draw(28/4*3, (ofGetHeight()/2 + 45)/4*3, cameraButton[2].getWidth()/4*3, cameraButton[2].getHeight()/4*3);
    }
    
    if(cameraButtonPressedIndex == 3) {//RIGHT
        //cameraButtonPressed[3].draw(106, ofGetHeight()/2 + 45);
        cameraButtonPressed[3].draw(106/4*3, (ofGetHeight()/2 + 45)/4*3, cameraButtonPressed[3].getWidth()/4*3, cameraButtonPressed[3].getHeight()/4*3);
    }
    else {
        //cameraButton[3].draw(106, ofGetHeight()/2 + 45);
        cameraButton[3].draw(106/4*3, (ofGetHeight()/2 + 45)/4*3, cameraButton[3].getWidth()/4*3, cameraButton[3].getHeight()/4*3);
    }
    
    if(cameraButtonPressedIndex == 4) { //UP
        //cameraButtonPressed[4].draw(67, ofGetHeight()/2 + 5);
        cameraButtonPressed[4].draw(67/4*3, (ofGetHeight()/2 + 5)/4*3, cameraButtonPressed[4].getWidth()/4*3, cameraButtonPressed[4].getHeight()/4*3);
    }
    else {
        //cameraButton[4].draw(67, ofGetHeight()/2 + 5);
        cameraButton[4].draw(67/4*3, (ofGetHeight()/2 + 5)/4*3, cameraButton[4].getWidth()/4*3, cameraButton[4].getHeight()/4*3);
    }
    
    if(cameraButtonPressedIndex == 5) {// DOWN
        //cameraButtonPressed[5].draw(67, ofGetHeight()/2 + 81);
        cameraButtonPressed[5].draw(67/4*3, (ofGetHeight()/2 + 81)/4*3, cameraButtonPressed[5].getWidth()/4*3, cameraButtonPressed[5].getHeight()/4*3);
    }
    else {
        //cameraButton[5].draw(67/4*3, (ofGetHeight()/2 + 81)/4*3);
        cameraButton[5].draw(67/4*3, (ofGetHeight()/2 + 81)/4*3, cameraButton[5].getWidth()/4*3, cameraButton[5].getHeight()/4*3);
    }
}

void ofApp::drawHeaders() {
    for(int i = 0; i < sizeof(headerMenu)/sizeof(headerMenu[0]); i++) {
        if(headerHoverIndex == i) {
            if(i != 0) {
                //headerHover[i].draw(ofGetWidth() - 200 + headerHover[i-1].getWidth(), 20);
                headerHover[i].draw(ofGetWidth() - 200 + (headerHover[i-1].getWidth())/4*3, 20, headerHover[i].getWidth()/4*3, headerHover[i].getHeight()/4*3);
            }
            else {
                //headerHover[i].draw(ofGetWidth() - 200, 20);
                headerHover[i].draw(ofGetWidth() - 200, 20, headerHover[i].getWidth()/4*3, headerHover[i].getHeight()/4*3);
            }
        }
        else {
            if(i != 0) {
                //headerMenu[i].draw(ofGetWidth() - 200 + headerMenu[i-1].getWidth(), 20);
                headerMenu[i].draw(ofGetWidth() - 200 + (headerMenu[i-1].getWidth())/4*3, 20, headerMenu[i].getWidth()/4*3, headerMenu[i].getHeight()/4*3);
            }
            else {
                //headerMenu[i].draw(ofGetWidth() - 200, 20);
                headerMenu[i].draw(ofGetWidth() - 200, 20, headerMenu[i].getWidth()/4*3, headerMenu[i].getHeight()/4*3);
            }
        }
    }
    
    switch(headerHoverIndex) {
        case 0 : {
            //float currentYPos = 20 + headerMenu[0].getHeight();
            float currentYPos = 20 + (headerMenu[0].getHeight())/4*3;
            for(int i = 0; i < sizeof(subMenuFile)/sizeof(subMenuFile[0]); i++) {
                if(subFileHoverIndex==i) {
                    //subFileHover[i].draw(ofGetWidth() - 200, currentYPos);
                    subFileHover[i].draw(ofGetWidth() - 200, currentYPos, subFileHover[i].getWidth()/4*3, subFileHover[i].getHeight()/4*3);
                    //currentYPos += subFileHover[i].getHeight();
                    currentYPos += subFileHover[i].getHeight()/4*3;
                }
                else {
                    subMenuFile[i].draw(ofGetWidth() - 200, currentYPos, subMenuFile[i].getWidth()/4*3, subMenuFile[i].getHeight()/4*3);
                    //currentYPos += subMenuFile[i].getHeight();
                    currentYPos += subMenuFile[i].getHeight()/4*3;
                }
            }
            break;
        }
        case 1 : {
            //float currentYPos = 20 + headerMenu[0].getHeight();
            float currentYPos = 20 + (headerMenu[0].getHeight())/4*3;
            for(int i = 0; i < sizeof(subMenuModeling)/sizeof(subMenuModeling[0]); i++) {
                if(subModelingHoverIndex==i) {
                    //subModelingHover[i].draw(ofGetWidth() - 200, currentYPos);
                    subModelingHover[i].draw(ofGetWidth() - 200, currentYPos, subModelingHover[i].getWidth()/4*3, subModelingHover[i].getHeight()/4*3);
                    //currentYPos += subModelingHover[i].getHeight();
                    currentYPos += subModelingHover[i].getHeight()/4*3;
                }
                else {
                    //subMenuModeling[i].draw(ofGetWidth() - 200, currentYPos);
                    subMenuModeling[i].draw(ofGetWidth() - 200, currentYPos, subMenuModeling[i].getWidth()/4*3, subMenuModeling[i].getHeight()/4*3);
                    //currentYPos += subMenuModeling[i].getHeight();
                    currentYPos += subModelingHover[i].getHeight()/4*3;
                }
            }
            break;
        }
    }
}

void ofApp::drawPanel() {
    if(currentSelectedProjector != -1) {
        panelWindow.draw(10, ofGetHeight() - 200, 1000, 190);
        int currentIndex = 0;
        for(int i=0; i<projectors.size(); i++) {
            if(projectors[i]->isSetted) {
                if(projectors[i]->isSelected) {
                    panelLabelClick.draw(10 + (90 * currentIndex), ofGetHeight() - 200, 90, 20);
                    ofDrawBitmapString("Proj. " + ofToString(i), 25 + (90 * currentIndex), ofGetHeight() - 200 + 15);
                }
                else {
                    panelLabel.draw(10 + (90 * currentIndex), ofGetHeight() - 203, 90, 20);
                    ofDrawBitmapString("Proj. " + ofToString(i), 25 + (90 * currentIndex), ofGetHeight() - 200 + 12);
                }
                currentIndex++;
            }
        }
        
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
        
        //panelWindow.draw(660, ofGetHeight() - 200, 400, 190);
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
        ofDrawBitmapString(str, 680, 625);
    }
    else if(isModelingSelected==1) {
        panelWindow.draw(10, ofGetHeight() - 200, ofGetWidth() - 20, 190);
        
        ofDrawBitmapString("Modeling Width : ", ofGetWidth() - 350, ofGetHeight() - 150);
        ofDrawBitmapString("Modeling Height : ", ofGetWidth() - 350, ofGetHeight() - 140);
        
        ofDrawBitmapString(ofToString(modelWidth), ofGetWidth() - 200, ofGetHeight() - 150);
        ofDrawBitmapString(ofToString(modelHeight), ofGetWidth() - 200, ofGetHeight() - 140);
        
        float currentScale = model.getScale().x;
        float lengthRatio = (currentScale) / 50;
        float barLength = 450 * lengthRatio;
        ofDrawBitmapString("Scale Factor", 20, ofGetHeight() - 85);
        ofDrawBitmapString(ofToString(currentScale), 575, ofGetHeight() - 85);
        
        panelGrayBar.draw(120, ofGetHeight() - 95, 450, 10);
        panelGreenBar.draw(120, ofGetHeight() - 95, barLength, 10);
        panelSliderButton.draw(120 + barLength - 10, ofGetHeight() - 100, 20, 20);
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
            /*if(projectors[i]->isSetted) {
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
            }*/
            if(projectors[i]->isSetted) {
                textFile << i << ',';
                textFile << projectors[i]->xPos << ',' << projectors[i]->yPos << ',' << projectors[i]->zPos << ',';
                textFile << projectors[i]->xRotation << ',' << projectors[i]->yRotation << ',' << projectors[i]->zRotation << ',';
                textFile << projectors[i]->pt[0].x << ',' << projectors[i]->pt[0].y << ',';
                textFile << projectors[i]->pt[1].x << ',' << projectors[i]->pt[1].y << ',';
                textFile << projectors[i]->pt[2].x << ',' << projectors[i]->pt[2].y << ',';
                textFile << projectors[i]->pt[3].x << ',' << projectors[i]->pt[3].y << ',';
                textFile << projectors[i]->manufacturerName << ',';
                textFile << projectors[i]->modelName << ',';
                textFile << projectors[i]->pivotDistance << ',';
                textFile << projectors[i]->pivotWidth << ',';
                textFile << projectors[i]->pivotHeight << ',';
                if(projectors[i]->videoPlayer.isLoaded())
                    textFile << projectors[i]->currentVideoURL << ',';
                else
                    textFile << "no video" << ',';
                if(projectors[i]->isMappingOn)
                    textFile << "mapped";
                else
                    textFile << "not mapped";
                textFile << endl;
            }
        }
        textFile.close();
    }
}

void ofApp::load() {
    ofFile textFile;
    ofBuffer buffer;
    vector<string> saveList;
    string str;
    ofFileDialogResult loadFileResult = ofSystemLoadDialog("load file");
    if(loadFileResult.bSuccess) {
        textFile.open(loadFileResult.getPath(), ofFile::ReadOnly);
        buffer = textFile.readToBuffer();
        for(auto line : buffer.getLines())
            saveList.push_back(line);
        
        if(saveList[0].compare("no modeling") != 0)
            open3DFile(saveList[0]);
        
        for(int i=1; i<saveList.size()-1; i++) {
            str = ofSplitString(saveList[i], ",")[0];
            int index = stoi(str);
            ofLog() << str;
            projectors[index]->isSetted = true;
            
            str = ofSplitString(saveList[i], ",")[1];
            projectors[index]->xPos = stof(str);
            str = ofSplitString(saveList[i], ",")[2];
            projectors[index]->yPos = stof(str);
            str = ofSplitString(saveList[i], ",")[3];
            projectors[index]->zPos = stof(str);
            
            str = ofSplitString(saveList[i], ",")[4];
            projectors[index]->xRotation = stof(str);
            str = ofSplitString(saveList[i], ",")[5];
            projectors[index]->yRotation = stof(str);
            str = ofSplitString(saveList[i], ",")[6];
            projectors[index]->zRotation = stof(str);
            
            str = ofSplitString(saveList[i], ",")[7];
            projectors[index]->pt[0].x = stof(str);
            str = ofSplitString(saveList[i], ",")[8];
            projectors[index]->pt[0].y = stof(str);
            str = ofSplitString(saveList[i], ",")[9];
            projectors[index]->pt[1].x = stof(str);
            str = ofSplitString(saveList[i], ",")[10];
            projectors[index]->pt[1].y = stof(str);
            str = ofSplitString(saveList[i], ",")[11];
            projectors[index]->pt[2].x = stof(str);
            str = ofSplitString(saveList[i], ",")[12];
            projectors[index]->pt[2].y = stof(str);
            str = ofSplitString(saveList[i], ",")[13];
            projectors[index]->pt[3].x = stof(str);
            str = ofSplitString(saveList[i], ",")[14];
            projectors[index]->pt[3].y = stof(str);
            
            str = ofSplitString(saveList[i], ",")[15];
            projectors[index]->manufacturerName = str;
            str = ofSplitString(saveList[i], ",")[16];
            projectors[index]->modelName = str;
            
            str = ofSplitString(saveList[i], ",")[17];
            projectors[index]->pivotDistance = stof(str);
            str = ofSplitString(saveList[i], ",")[18];
            projectors[index]->pivotWidth = stof(str);
            str = ofSplitString(saveList[i], ",")[19];
            projectors[index]->pivotHeight = stof(str);
            
            str = ofSplitString(saveList[i], ",")[20];
            if(str.compare("no video") != 0) {
                projectors[index]->currentVideoURL = str;
                projectors[index]->videoPlayer.load(str);
                projectors[index]->videoPlayer.play();
            }
            
            str = ofSplitString(saveList[i], ",")[21];
            if(str.compare("mapped") == 0)
                projectors[index]->isMappingOn = true;
        }
        
        //open3DFile(openFileResult);
        /*textFile.open(loadFileResult.getPath(), ofFile::ReadOnly);
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
        }*/
    }
    textFile.close();
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
    if(model.getMeshCount()!=0) {
        ofVec3f modelPt = easyCam.worldToScreen(model.getPosition());
        float distance = modelPt.distance(mousePt);
        if(distance < 100) {
            //ofLog() << distance;
            isModelingSelected *= -1;
            return nearestIndex;
        }
    }
    for(int i=0; i<projectors.size(); i++) {
        ofVec3f projectorPt = easyCam.worldToScreen(ofVec3f(projectors[i]->xPos, projectors[i]->yPos, projectors[i]->zPos));
        float distance = projectorPt.distance(mousePt);
        if(distance < 50 && distance < nearestDistance) {
            nearestDistance = distance;
            if(projectors[i]->isSetted) {
                nearestIndex = i;
                isModelingSelected = -1;
            }
        }
    }
    return nearestIndex;
}

void ofApp::scaleModeling(float factor) {
    model.setScale(model.getScale().x + factor, model.getScale().x + factor, model.getScale().x + factor);
    reconstructMesh();
}
