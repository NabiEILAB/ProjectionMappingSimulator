#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(70,70,70);
    ofEnableDepthTest(); //draw things in z value order
    
    //allocating new projector class - 7 projectors only, due to the performance issue
    //                                (still, leave it as dynamic vector form, just in case)
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    projectors.push_back(new Projector(0, 0, 0, 0, 0, 0, projectors.size()));
    
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->setup();
    
    //maximum camera rendering distance limit
    easyCam.setFarClip(12000);
    easyCam.setPosition(0,200,600);
    easyCam.setTarget(ofVec3f(0,0,0));
    easyCam.disableMouseInput();
    
    //load shaders
    textureProjectionShader.load("TextureProjection"); //'Projection Mapping' shader
    depthStoringShader.load("DepthStoring");           //'Shadow Map' shader
    projectorTextureShader.load("ProjectorTexture");   //basic lighting on projector modeling
    modelingBasicShader.load("ModelingBasic");         //basic lighting on imported modeling
    
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
    cameraMenu.load("UI/DropDown/chooseMenu.png");
    cameraMenuClicked.load("UI/DropDown/chooseMenuClick.png");
    cameraTopMenu.load("UI/DropDown/topMenu.png");
    cameraTopMenuHover.load("UI/DropDown/topMenuHover.png");
    cameraCenterMenu.load("UI/DropDown/centerMenu.png");
    cameraCenterMenuHover.load("UI/DropDown/centerMenuHover.png");
    cameraBottomMenu.load("UI/DropDown/bottomMenu.png");
    cameraBottomMenuHover.load("UI/Dropdown/bottomMenuHover.png");
    cameraButtonPressedIndex = -1;
    cameraModeHoverIndex = -1;
    currentCameraModeIndex = 0;
    isCameraMenuClicked = false;
    
    
    panelWindow.load("UI/Panel/panelWindow.png");
    panelGrayBar.load("UI/Panel/grayBar.png");
    panelGreenBar.load("UI/Panel/greenBar.png");
    panelSliderButton.load("UI/Panel/sliderButton.png");
    panelLabel.load("UI/Panel/label.png");
    panelLabelClick.load("UI/Panel/labelClick.png");
    panelButtonPlus.load("UI/Button/+ deselect.png");
    panelButtonPlusPressed.load("UI/Button/+ select.png");
    panelButtonMinus.load("UI/Button/- deselect.png");
    panelButtonMinusPressed.load("UI/Button/- select.png");
    panelClickIndex = -1;
    panelPlusClickIndex = -1;
    panelMinusClickIndex = -1;
    
    currentModelURL = "";
    
    projectorModel.loadModel("projector.3ds");
    reconstructProjectorMesh();
    
    ofAddListener(mappingWindow->events().exit, this, &ofApp::closeApp);
    exiting = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0; i<projectors.size(); i++)
        projectors[i]->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    easyCam.begin();
    
    settingCameraPosition();
    
    ofDisableDepthTest();
    ofDrawGrid(200, 15, true, false, true, false);
    ofEnableDepthTest();
    
    renderCustomModel();
    renderProjectorModel();
    
    easyCam.end();
    //ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
    
    ofDisableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    //gui.draw();
    drawPanel();
    drawButtons();
    ofEnableDepthTest();
    
    drawHeaders();
    drawDropDownMenus();
    ofDisableBlendMode();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 'e' : {
            cameraButtonPressedIndex = 0;
            break;
        }
        case 'q' : {
            cameraButtonPressedIndex = 1;
            break;
        }
        case 'a' : {
            cameraButtonPressedIndex = 2;
            break;
        }
        case 'd' : {
            cameraButtonPressedIndex = 3;
            break;
        }
        case 'w' : {
            cameraButtonPressedIndex = 4;
            break;
        }
        case 's' : {
            cameraButtonPressedIndex = 5;
            break;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    cameraButtonPressedIndex = -1;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    dropDownMoveEventCheck(x,y);
    headerMoveEventCheck(x,y);
    cameraMoveEventCheck(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    panelDragEventCheck(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    dropDownClickEventCheck(x, y, button);
    cameraClickEventCheck(x, y, button);
    panelClickEventCheck(x, y, button);
    labelClickEventCheck(x, y, button);
    headerClickEventCheck(x, y, button);
    subClickEventCheck(x, y, button);
    objectClickEventCheck(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if(cameraButtonPressedIndex != -1)
        cameraButtonPressedIndex = -1;
    panelReleaseEventCheck(x, y, button);
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
//--------------------------------------------------------------
void ofApp::exit() {
    if(!exiting) {
        exiting = true;
        ofExit();
    }
}

//Additional function
/*void ofApp::refreshGUI() {
    gui.clear();
    projectorPanelGroup.clear();
    for(int i=0; i<projectors.size(); i++) {
        if(projectors[i]->isSelected) {
            projectorPanelGroup.add(projectors[i]->projectorParameters);
        }
    }
    gui.setup(projectorPanelGroup);
}*/

void ofApp::addProjector(float x, float y, float z) {
    for(int i=0; i<projectors.size(); i++) {
        if(!projectors[i]->isSetted) {
            if(currentSelectedProjector != -1)
                projectors[currentSelectedProjector]->isSelected = false;
            projectors[i]->activate(x, y, z);
            currentSelectedProjector = i;
            //mappingGUI->setProjector(projectors[currentSelectedProjector]);
            break;
        }
    }
}

void ofApp::deleteProjector(int projectorNum) {
    projectors[projectorNum]->deactivate();
    currentSelectedProjector = -1;
    //refreshGUI();
}

void ofApp::settingCameraPosition() {
    if(cameraButtonPressedIndex==0) { //ZOOM IN
        //easyCam.disableMouseInput();
        float x = easyCam.getTarget().getX() + easyCam.getLookAtDir().x * 10;
        float y = easyCam.getTarget().getY() + easyCam.getLookAtDir().y * 10;
        float z = easyCam.getTarget().getZ() + easyCam.getLookAtDir().z * 10;
        easyCam.setTarget(ofVec3f(x,y,z));
        easyCam.dolly(-10);
    }
    else if(cameraButtonPressedIndex==1) { //ZOOM OUT
        //easyCam.disableMouseInput();
        float x = easyCam.getTarget().getX() - easyCam.getLookAtDir().x * 10;
        float y = easyCam.getTarget().getY() - easyCam.getLookAtDir().y * 10;
        float z = easyCam.getTarget().getZ() - easyCam.getLookAtDir().z * 10;
        easyCam.setTarget(ofVec3f(x,y,z));
        easyCam.dolly(10);
    }
    else if(cameraButtonPressedIndex==2) { //MOVE LEFT & ROTATE LEFT & ORBIT LEFT
        //easyCam.disableMouseInput();
        if(currentCameraModeIndex==0) {
            easyCam.move(easyCam.getSideDir() * -10);
            float x = easyCam.getTarget().getX() - easyCam.getSideDir().x * 10;
            float y = easyCam.getTarget().getY() - easyCam.getSideDir().y * 10;
            float z = easyCam.getTarget().getZ() - easyCam.getSideDir().z * 10;
            easyCam.setTarget(ofVec3f(x,y,z));
        }
        else if(currentCameraModeIndex==1) {
            float targetToOriginX = easyCam.getTarget().getX() - easyCam.getPosition().x;
            float targetToOriginY = easyCam.getTarget().getY() - easyCam.getPosition().y;
            float targetToOriginZ = easyCam.getTarget().getZ() - easyCam.getPosition().z;
            
            float ux = -easyCam.getYAxis().x;
            float uy = -easyCam.getYAxis().y;
            float uz = -easyCam.getYAxis().z;
            
            float theta = -0.05;
            
            float rotatedX = ((cos(theta) + ux * ux * (1 - cos(theta))) * targetToOriginX + (ux * uy * (1 - cos(theta)) - uz * sin(theta)) * targetToOriginY + (ux * uz * (1 - cos(theta)) + uy * sin(theta)) * targetToOriginZ) + easyCam.getPosition().x;
            float rotatedY = ((uy * ux * (1 - cos(theta)) + uz * sin(theta)) * targetToOriginX + (cos(theta) + uy * uy * (1 - cos(theta))) * targetToOriginY + (uy * uz * (1 - cos(theta)) - ux * sin(theta)) * targetToOriginZ) + easyCam.getPosition().y;
            float rotatedZ = ((uz * ux * (1 - cos(theta)) - uy * sin(theta)) * targetToOriginX + (uz * uy * (1 - cos(theta)) + ux * sin(theta)) * targetToOriginY + (cos(theta) + uz * uz * (1 - cos(theta))) * targetToOriginZ) + easyCam.getPosition().z;
            
            easyCam.setTarget(ofVec3f(rotatedX, rotatedY, rotatedZ));
            
        }
        else if(currentCameraModeIndex==2){
            float camToOriginX = easyCam.getPosition().x - easyCam.getTarget().getX();
            float camToOriginY = easyCam.getPosition().y - easyCam.getTarget().getY();
            float camToOriginZ = easyCam.getPosition().z - easyCam.getTarget().getZ();
            
            float ux = easyCam.getYAxis().x;
            float uy = easyCam.getYAxis().y;
            float uz = easyCam.getYAxis().z;
            
            float theta = -0.05;
            
            float rotatedX = ((cos(theta) + ux * ux * (1 - cos(theta))) * camToOriginX + (ux * uy * (1 - cos(theta)) - uz * sin(theta)) * camToOriginY + (ux * uz * (1 - cos(theta)) + uy * sin(theta)) * camToOriginZ) + easyCam.getTarget().getX();
            float rotatedY = ((uy * ux * (1 - cos(theta)) + uz * sin(theta)) * camToOriginX + (cos(theta) + uy * uy * (1 - cos(theta))) * camToOriginY + (uy * uz * (1 - cos(theta)) - ux * sin(theta)) * camToOriginZ) + easyCam.getTarget().getY();
            float rotatedZ = ((uz * ux * (1 - cos(theta)) - uy * sin(theta)) * camToOriginX + (uz * uy * (1 - cos(theta)) + ux * sin(theta)) * camToOriginY + (cos(theta) + uz * uz * (1 - cos(theta))) * camToOriginZ) + easyCam.getTarget().getZ();
            
            easyCam.setPosition(rotatedX, rotatedY, rotatedZ);
            easyCam.setTarget(easyCam.getTarget());
        }

    }
    else if(cameraButtonPressedIndex==3) { //MOVE RIGHT & ROTATE RIGHT & ORBIT RIGHT
        //easyCam.disableMouseInput();
        if(currentCameraModeIndex==0) {
            easyCam.move(easyCam.getSideDir() * 10);
            float x = easyCam.getTarget().getX() + easyCam.getSideDir().x * 10;
            float y = easyCam.getTarget().getY() + easyCam.getSideDir().y * 10;
            float z = easyCam.getTarget().getZ() + easyCam.getSideDir().z * 10;
            easyCam.setTarget(ofVec3f(x, y, z));
        }
        else if(currentCameraModeIndex==1){
            float targetToOriginX = easyCam.getTarget().getX() - easyCam.getPosition().x;
            float targetToOriginY = easyCam.getTarget().getY() - easyCam.getPosition().y;
            float targetToOriginZ = easyCam.getTarget().getZ() - easyCam.getPosition().z;
            
            float ux = -easyCam.getYAxis().x;
            float uy = -easyCam.getYAxis().y;
            float uz = -easyCam.getYAxis().z;
            
            float theta = 0.05;
            
            float rotatedX = ((cos(theta) + ux * ux * (1 - cos(theta))) * targetToOriginX + (ux * uy * (1 - cos(theta)) - uz * sin(theta)) * targetToOriginY + (ux * uz * (1 - cos(theta)) + uy * sin(theta)) * targetToOriginZ) + easyCam.getPosition().x;
            float rotatedY = ((uy * ux * (1 - cos(theta)) + uz * sin(theta)) * targetToOriginX + (cos(theta) + uy * uy * (1 - cos(theta))) * targetToOriginY + (uy * uz * (1 - cos(theta)) - ux * sin(theta)) * targetToOriginZ) + easyCam.getPosition().y;
            float rotatedZ = ((uz * ux * (1 - cos(theta)) - uy * sin(theta)) * targetToOriginX + (uz * uy * (1 - cos(theta)) + ux * sin(theta)) * targetToOriginY + (cos(theta) + uz * uz * (1 - cos(theta))) * targetToOriginZ) + easyCam.getPosition().z;
            
            
            easyCam.setTarget(ofVec3f(rotatedX, rotatedY, rotatedZ));
        }
        else if(currentCameraModeIndex==2) {
            float camToOriginX = easyCam.getPosition().x - easyCam.getTarget().getX();
            float camToOriginY = easyCam.getPosition().y - easyCam.getTarget().getY();
            float camToOriginZ = easyCam.getPosition().z - easyCam.getTarget().getZ();
            
            float ux = easyCam.getYAxis().x;
            float uy = easyCam.getYAxis().y;
            float uz = easyCam.getYAxis().z;
            
            float theta = 0.05;
            
            float rotatedX = ((cos(theta) + ux * ux * (1 - cos(theta))) * camToOriginX + (ux * uy * (1 - cos(theta)) - uz * sin(theta)) * camToOriginY + (ux * uz * (1 - cos(theta)) + uy * sin(theta)) * camToOriginZ) + easyCam.getTarget().getX();
            float rotatedY = ((uy * ux * (1 - cos(theta)) + uz * sin(theta)) * camToOriginX + (cos(theta) + uy * uy * (1 - cos(theta))) * camToOriginY + (uy * uz * (1 - cos(theta)) - ux * sin(theta)) * camToOriginZ) + easyCam.getTarget().getY();
            float rotatedZ = ((uz * ux * (1 - cos(theta)) - uy * sin(theta)) * camToOriginX + (uz * uy * (1 - cos(theta)) + ux * sin(theta)) * camToOriginY + (cos(theta) + uz * uz * (1 - cos(theta))) * camToOriginZ) + easyCam.getTarget().getZ();
            
            easyCam.setPosition(rotatedX, rotatedY, rotatedZ);
            easyCam.setTarget(easyCam.getTarget());
        }
    }
    else if(cameraButtonPressedIndex==4) { //MOVE UP & ROTATE UP & ORBIT UP
        //easyCam.disableMouseInput();
        if(currentCameraModeIndex==0) {
            easyCam.move(easyCam.getUpDir() * 10);
            float x = easyCam.getTarget().getX() + easyCam.getUpDir().x * 10;
            float y = easyCam.getTarget().getY() + easyCam.getUpDir().y * 10;
            float z = easyCam.getTarget().getZ() + easyCam.getUpDir().z * 10;
            easyCam.setTarget(ofVec3f(x, y, z));
        }
        else if(currentCameraModeIndex==1) {
            float targetToOriginX = easyCam.getTarget().getX() - easyCam.getPosition().x;
            float targetToOriginY = easyCam.getTarget().getY() - easyCam.getPosition().y;
            float targetToOriginZ = easyCam.getTarget().getZ() - easyCam.getPosition().z;
            
            float ux = -easyCam.getXAxis().x;
            float uy = -easyCam.getXAxis().y;
            float uz = -easyCam.getXAxis().z;
            
            float theta = -0.05;
            float rotatedX = ((cos(theta) + ux * ux * (1 - cos(theta))) * targetToOriginX + (ux * uy * (1 - cos(theta)) - uz * sin(theta)) * targetToOriginY + (ux * uz * (1 - cos(theta)) + uy * sin(theta)) * targetToOriginZ) + easyCam.getPosition().x;
            float rotatedY = ((uy * ux * (1 - cos(theta)) + uz * sin(theta)) * targetToOriginX + (cos(theta) + uy * uy * (1 - cos(theta))) * targetToOriginY + (uy * uz * (1 - cos(theta)) - ux * sin(theta)) * targetToOriginZ) + easyCam.getPosition().y;
            float rotatedZ = ((uz * ux * (1 - cos(theta)) - uy * sin(theta)) * targetToOriginX + (uz * uy * (1 - cos(theta)) + ux * sin(theta)) * targetToOriginY + (cos(theta) + uz * uz * (1 - cos(theta))) * targetToOriginZ) + easyCam.getPosition().z;
            
            easyCam.setTarget(ofVec3f(rotatedX, rotatedY, rotatedZ));
            if ((ux > 0 && easyCam.getXAxis().x > 0) || (ux < 0 && easyCam.getXAxis().x < 0))
                easyCam.roll(180);
        }
        else if(currentCameraModeIndex==2) {
            float camToOriginX = easyCam.getPosition().x - easyCam.getTarget().getX();
            float camToOriginY = easyCam.getPosition().y - easyCam.getTarget().getY();
            float camToOriginZ = easyCam.getPosition().z - easyCam.getTarget().getZ();
            
            float ux = easyCam.getXAxis().x;
            float uy = easyCam.getXAxis().y;
            float uz = easyCam.getXAxis().z;
            
            float theta = -0.05;
            
            float rotatedX = ((cos(theta) + ux * ux * (1 - cos(theta))) * camToOriginX + (ux * uy * (1 - cos(theta)) - uz * sin(theta)) * camToOriginY + (ux * uz * (1 - cos(theta)) + uy * sin(theta)) * camToOriginZ) + easyCam.getTarget().getX();
            float rotatedY = ((uy * ux * (1 - cos(theta)) + uz * sin(theta)) * camToOriginX + (cos(theta) + uy * uy * (1 - cos(theta))) * camToOriginY + (uy * uz * (1 - cos(theta)) - ux * sin(theta)) * camToOriginZ) + easyCam.getTarget().getY();
            float rotatedZ = ((uz * ux * (1 - cos(theta)) - uy * sin(theta)) * camToOriginX + (uz * uy * (1 - cos(theta)) + ux * sin(theta)) * camToOriginY + (cos(theta) + uz * uz * (1 - cos(theta))) * camToOriginZ) + easyCam.getTarget().getZ();
            
            easyCam.setPosition(rotatedX, rotatedY, rotatedZ);
            easyCam.setTarget(easyCam.getTarget());
            
            if ((ux > 0 && easyCam.getXAxis().x < 0) || (ux < 0 && easyCam.getXAxis().x > 0))
                easyCam.roll(180);
        }
    }
    else if(cameraButtonPressedIndex==5) { //MOVE DOWN & ROTATE DOWN & ORBIT DOWN
        //easyCam.disableMouseInput();
        if(currentCameraModeIndex==0) {
            easyCam.move(easyCam.getUpDir() * -10);
            float x = easyCam.getTarget().getX() - easyCam.getUpDir().x * 10;
            float y = easyCam.getTarget().getY() - easyCam.getUpDir().y * 10;
            float z = easyCam.getTarget().getZ() - easyCam.getUpDir().z * 10;
            easyCam.setTarget(ofVec3f(x, y, z));
        }
        else if(currentCameraModeIndex==1) {
            float targetToOriginX = easyCam.getTarget().getX() - easyCam.getPosition().x;
            float targetToOriginY = easyCam.getTarget().getY() - easyCam.getPosition().y;
            float targetToOriginZ = easyCam.getTarget().getZ() - easyCam.getPosition().z;
            
            float ux = -easyCam.getXAxis().x;
            float uy = -easyCam.getXAxis().y;
            float uz = -easyCam.getXAxis().z;
            
            float theta = 0.05;
            
            float rotatedX = ((cos(theta) + ux * ux * (1 - cos(theta))) * targetToOriginX + (ux * uy * (1 - cos(theta)) - uz * sin(theta)) * targetToOriginY + (ux * uz * (1 - cos(theta)) + uy * sin(theta)) * targetToOriginZ) + easyCam.getPosition().x;
            float rotatedY = ((uy * ux * (1 - cos(theta)) + uz * sin(theta)) * targetToOriginX + (cos(theta) + uy * uy * (1 - cos(theta))) * targetToOriginY + (uy * uz * (1 - cos(theta)) - ux * sin(theta)) * targetToOriginZ) + easyCam.getPosition().y;
            float rotatedZ = ((uz * ux * (1 - cos(theta)) - uy * sin(theta)) * targetToOriginX + (uz * uy * (1 - cos(theta)) + ux * sin(theta)) * targetToOriginY + (cos(theta) + uz * uz * (1 - cos(theta))) * targetToOriginZ) + easyCam.getPosition().z;
            
            easyCam.setTarget(ofVec3f(rotatedX, rotatedY, rotatedZ));
            if ((ux > 0 && easyCam.getXAxis().x > 0) || (ux < 0 && easyCam.getXAxis().x < 0))
                easyCam.roll(180);
        }
        else if(currentCameraModeIndex==2){
            float camToOriginX = easyCam.getPosition().x - easyCam.getTarget().getX();
            float camToOriginY = easyCam.getPosition().y - easyCam.getTarget().getY();
            float camToOriginZ = easyCam.getPosition().z - easyCam.getTarget().getZ();
            
            float ux = easyCam.getXAxis().x;
            float uy = easyCam.getXAxis().y;
            float uz = easyCam.getXAxis().z;
            
            float theta = 0.05;
            
            float rotatedX = ((cos(theta) + ux * ux * (1 - cos(theta))) * camToOriginX + (ux * uy * (1 - cos(theta)) - uz * sin(theta)) * camToOriginY + (ux * uz * (1 - cos(theta)) + uy * sin(theta)) * camToOriginZ) + easyCam.getTarget().getX();
            float rotatedY = ((uy * ux * (1 - cos(theta)) + uz * sin(theta)) * camToOriginX + (cos(theta) + uy * uy * (1 - cos(theta))) * camToOriginY + (uy * uz * (1 - cos(theta)) - ux * sin(theta)) * camToOriginZ) + easyCam.getTarget().getY();
            float rotatedZ = ((uz * ux * (1 - cos(theta)) - uy * sin(theta)) * camToOriginX + (uz * uy * (1 - cos(theta)) + ux * sin(theta)) * camToOriginY + (cos(theta) + uz * uz * (1 - cos(theta))) * camToOriginZ) + easyCam.getTarget().getZ();
            
            easyCam.setPosition(rotatedX, rotatedY, rotatedZ);
            easyCam.setTarget(easyCam.getTarget());
            
            if ((ux > 0 && easyCam.getXAxis().x < 0) || (ux < 0 && easyCam.getXAxis().x > 0))
                easyCam.roll(180);
        }
    }
    else {
        /*if(!easyCam.getMouseInputEnabled()) {
            easyCam.enableMouseInput();
            if(panelClickIndex == -1) {
                ofNode t = easyCam.getTarget();
                ofVec3f p = easyCam.getPosition();
                float r = easyCam.getRoll();
                ofVec3f v = easyCam.getXAxis();
                easyCam.reset();
                easyCam.setPosition(p.x, p.y, p.z);
                easyCam.setTarget(t);
                if(v!=easyCam.getXAxis())
                    easyCam.roll(180);
            }
        }*/
    }
}

void ofApp::renderCustomModel() {
    /////////////////////// 1. Make Modeling render scene in FBO ///////////////////////
    ofFbo fbo;
    ofFbo::Settings settings;
    settings.width = ofGetWidth();
    settings.height = ofGetHeight();
    settings.textureTarget = GL_TEXTURE_2D;
    settings.internalformat = GL_RGBA32F_ARB;
    settings.useDepth = true;
    settings.depthStencilAsTexture = true;
    settings.useStencil = true;
    fbo.allocate(settings);
    
    fbo.begin();
    ofClear(0);
    ofSetColor(255,255,255,255);
    //fbo.getTexture().draw(0,0);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    easyCam.end();
    ofDisableDepthTest();
    ofBackground(70,70,70);
    ofDrawGrid(200, 15, true, false, true, false);
    ofEnableDepthTest();
    easyCam.begin();
    
    ofMatrix4x4 modelMatrix = ofMatrix4x4::newIdentityMatrix();
    modelMatrix.translate(0, 0, 0);
    
    modelingBasicShader.begin();
    modelingBasicShader.setUniformMatrix4f("modelMatrix", modelMatrix);
    modelingBasicShader.setUniform3f("lightPos", easyCam.getX(), easyCam.getY(), easyCam.getZ());
    modelingBasicShader.setUniform1f("isModelingSelected", (float)isModelingSelected);
    
    easyCam.end();
    for(int j=0; j<meshes.size(); j++)
        meshes[j].draw();
    easyCam.begin();
    
    
    modelingBasicShader.end();
    fbo.end();
    ////////////////////////////////////////////////////////////////////////////////////
    
    
    
    
    /////////////////////// 2. Make projection mapping render scene in each projector's FBO ///////////////////////
    for(int i=0; i<projectors.size(); i++) {
        if(projectors[i]->isSetted)
            projectors[i]->draw();
        
        if(!projectors[i]->videoPlayer.isLoaded())
            continue;
        
        /////////////////////// 2-1. Make 'Shadow Map' as Texture ///////////////////////
        projectors[i]->allocateShadowFbo();
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
        
        /////////////////////// 2-2. Make 'Projection Mapping scene' in FBO ///////////////////////
        setProjectorShader(i);
        
        projectors[i]->allocateResultFbo();
        projectors[i]->resultFbo.begin();
        projectors[i]->clearResultFbo();
        
        textureProjectionShader.begin();
        modelMatrix = ofMatrix4x4::newIdentityMatrix();
        modelMatrix.translate(0, 0, 0);
    
        ofMatrix4x4 bias = ofMatrix4x4::newIdentityMatrix();
        bias.scale(0.5, 0.5, 0.5);
        bias.translate(0.5, 0.5, 0.5);
    
        textureProjectionShader.setUniformMatrix4f("biasMatrix", bias);
        textureProjectionShader.setUniformMatrix4f("modelMatrix", modelMatrix);
        
        easyCam.end();
        for(int j=0; j<meshes.size(); j++)
            meshes[j].draw();
        easyCam.begin();
        
        textureProjectionShader.end();
        projectors[i]->resultFbo.end();
        
        projectors[i]->shadowFbo.clear();
        projectors[i]->textureFbo.clear();
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    
    /////////////////////// 3. Blend whole FBOs ///////////////////////
    if(model.getMeshCount() > 0) {
        easyCam.end();
        ofDisableDepthTest();
        fbo.draw(0,0);
        fbo.clear();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        for(int i=0; i<projectors.size(); i++) {
            if(projectors[i]->isSetted && projectors[i]->videoPlayer.isLoaded())
                projectors[i]->resultFbo.draw(0,0);
        }
        
        ofDisableBlendMode();
        ofEnableDepthTest();
        easyCam.begin();
        
        ofDrawLine(modelPivotPoint.x,0,modelPivotPoint.z,modelPivotPoint.x,modelHeight,modelPivotPoint.z);
        ofDrawBitmapString("Calculate distance from here", modelPivotPoint.x,modelHeight,modelPivotPoint.z);
    }
    /////////////////////////////////////////////////////////////////
    
    for(int i=0; i<projectors.size(); i++)
        if(projectors[i]->videoPlayer.isLoaded())
            projectors[i]->resultFbo.clear();
}

void ofApp::renderProjectorModel() {
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
    
    //model.setPosition(-leftestX,-lowestY,-closestZ);
    model.setPosition(0, -lowestY, -closestZ);
    
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
            //vert += ofVec3f(-leftestX, -lowestY, -closestZ);
            vert += ofVec3f(0, -lowestY, -closestZ);
            ofVec3f& norm = meshes[i].getNormals()[j];
            //norm += ofVec3f(-leftestX, -lowestY, -closestZ);
            norm += ofVec3f(0, -lowestY, -closestZ);
        }
    }
    
    float bottomLeftestX = 3000;
    float bottomClosestZ = -3000;
    vector<ofVec3f> yCandidates;
    vector<ofVec3f> zCandidates;
    for(int i = 0; i < model.getMeshCount(); i++) {
        for(int j=0; j<meshes[i].getNumVertices(); j++) {
            ofVec3f& vert = meshes[i].getVertices()[j];
            if(vert.y<15) {
                yCandidates.push_back(vert);
                if(vert.z > bottomClosestZ)
                    bottomClosestZ = vert.z;
            }
        }
    }
    for(int i=0; i<yCandidates.size(); i++) {
        if(yCandidates[i].z == bottomClosestZ) {
            zCandidates.push_back(yCandidates[i]);
            if(yCandidates[i].x < bottomLeftestX)
                bottomLeftestX = yCandidates[i].x;
        }
    }
    modelPivotPoint = ofPoint(bottomLeftestX, 0, bottomClosestZ);
    //modelPivotPoint = ofPoint(leftestX, 0, 0);
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
            projectors[currentSelectedProjector]->videoPlayer.stop();
            projectors[currentSelectedProjector]->videoPlayer.close();
            projectors[currentSelectedProjector]->videoPlayer.load(openFileResult.getPath());
            projectors[currentSelectedProjector]->videoPlayer.play();
            
            
            projectors[currentSelectedProjector]->currentVideoURL = openFileResult.getPath();
            mappingGUI->setProjector(projectors[currentSelectedProjector]);
        }
    }
}

void ofApp::setProjectorShader(int index) {
    textureProjectionShader.begin();
    textureProjectionShader.setUniformMatrix4f("projectorMatrix", projectors[index]->projectorMatrix);
    textureProjectionShader.setUniformMatrix4f("projectorProjectionMatrix", projectors[index]->projectorProjection);
    textureProjectionShader.setUniformMatrix4f("projectorViewMatrix", projectors[index]->projectorView);
    textureProjectionShader.setUniform3f("projectorPos", projectors[index]->xPos, projectors[index]->yPos, projectors[index]->zPos);
    textureProjectionShader.setUniformTexture("projectorTex", projectors[index]->textureFbo.getTexture(), 0);
    textureProjectionShader.setUniformTexture("shadowTex", projectors[index]->shadowFbo.getDepthTexture(), 1);
    textureProjectionShader.setUniform1f("radius", projectors[index]->height); //radius att test!
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
    cameraZoomBackground.draw(50/4*3, (ofGetHeight()/2 - 200)/4*3, cameraZoomBackground.getWidth()/4*3, cameraZoomBackground.getHeight()/4*3);
    
    for(int i=0; i<2; i++) {
        if(cameraButtonPressedIndex == i)
            cameraButtonPressed[i].draw(56/4*3, (ofGetHeight()/2 - 192 + (72 * i))/4*3, 56/4*3, 56/4*3);
        else
            cameraButton[i].draw(56/4*3, (ofGetHeight()/2 - 192 + (72 * i))/4*3, 56/4*3, 56/4*3);
    }
    
    cameraMoveBackground.draw(23/4*3, (ofGetHeight()/2)/4*3, cameraMoveBackground.getWidth()/4*3, cameraMoveBackground.getHeight()/4*3);
    
    if(cameraButtonPressedIndex == 2) //LEFT
        cameraButtonPressed[2].draw(28/4*3, (ofGetHeight()/2 + 45)/4*3, cameraButtonPressed[2].getWidth()/4*3, cameraButtonPressed[2].getHeight()/4*3);
    else
        cameraButton[2].draw(28/4*3, (ofGetHeight()/2 + 45)/4*3, cameraButton[2].getWidth()/4*3, cameraButton[2].getHeight()/4*3);
    
    if(cameraButtonPressedIndex == 3) //RIGHT
        cameraButtonPressed[3].draw(106/4*3, (ofGetHeight()/2 + 45)/4*3, cameraButtonPressed[3].getWidth()/4*3, cameraButtonPressed[3].getHeight()/4*3);
    else
        cameraButton[3].draw(106/4*3, (ofGetHeight()/2 + 45)/4*3, cameraButton[3].getWidth()/4*3, cameraButton[3].getHeight()/4*3);
    
    if(cameraButtonPressedIndex == 4) //UP
        cameraButtonPressed[4].draw(67/4*3, (ofGetHeight()/2 + 5)/4*3, cameraButtonPressed[4].getWidth()/4*3, cameraButtonPressed[4].getHeight()/4*3);
    else
        cameraButton[4].draw(67/4*3, (ofGetHeight()/2 + 5)/4*3, cameraButton[4].getWidth()/4*3, cameraButton[4].getHeight()/4*3);
    
    if(cameraButtonPressedIndex == 5) // DOWN
        cameraButtonPressed[5].draw(67/4*3, (ofGetHeight()/2 + 81)/4*3, cameraButtonPressed[5].getWidth()/4*3, cameraButtonPressed[5].getHeight()/4*3);
    else
        cameraButton[5].draw(67/4*3, (ofGetHeight()/2 + 81)/4*3, cameraButton[5].getWidth()/4*3, cameraButton[5].getHeight()/4*3);
    
    if(isCameraMenuClicked) {
        cameraMenuClicked.draw(23, (ofGetHeight()/2), cameraMenuClicked.getWidth()/2, cameraMenu.getHeight()/3*2);
        ofPushStyle();
        ofSetColor(255,255,255);
    }
    else {
        cameraMenu.draw(23, (ofGetHeight()/2), cameraMenu.getWidth()/2, cameraMenu.getHeight()/3*2);
        ofPushStyle();
        ofSetColor(0,0,0);
    }
    
    if(currentCameraModeIndex==0)
        ofDrawBitmapString("Translate", 25, ofGetHeight()/2 + 15);
    else if(currentCameraModeIndex==1)
        ofDrawBitmapString("Rotate", 30, ofGetHeight()/2 + 15);
    else if(currentCameraModeIndex==2)
        ofDrawBitmapString("Orbit", 30, ofGetHeight()/2 + 15);
    ofPopStyle();
    
    if(isCameraMenuClicked) {
        if(cameraModeHoverIndex==0) {
            cameraTopMenuHover.draw(23, ofGetHeight()/2 + 20, cameraMenu.getWidth()/2, cameraMenu.getHeight()/3*2);
            ofPushStyle();
            ofSetColor(255, 255, 255);
        }
        else {
            cameraTopMenu.draw(23, ofGetHeight()/2 + 20, cameraMenu.getWidth()/2, cameraMenu.getHeight()/3*2);
            ofPushStyle();
            ofSetColor(0, 0, 0);
        }
        ofDrawBitmapString("Translate", 25, ofGetHeight()/2 + 34);
        ofPopStyle();
        
        if(cameraModeHoverIndex==1) {
            cameraCenterMenuHover.draw(23, ofGetHeight()/2 + 20 + cameraMenu.getHeight()/3*2, cameraMenu.getWidth()/2, cameraMenu.getHeight()/3*2);
            ofPushStyle();
            ofSetColor(255, 255, 255);
        }
        else {
            cameraCenterMenu.draw(23, ofGetHeight()/2 + 20 + cameraMenu.getHeight()/3*2, cameraMenu.getWidth()/2, cameraMenu.getHeight()/3*2);
            ofPushStyle();
            ofSetColor(0, 0, 0);
        }
        ofDrawBitmapString("Rotate", 33, ofGetHeight()/2 + 34 + cameraMenu.getHeight()/3*2);
        ofPopStyle();
        
        if(cameraModeHoverIndex==2) {
            cameraBottomMenuHover.draw(23, ofGetHeight()/2 + 20 + cameraMenu.getHeight()/3*2 * 2, cameraMenu.getWidth()/2, cameraMenu.getHeight()/3*2);
            ofPushStyle();
            ofSetColor(255, 255, 255);
        }
        else {
            cameraBottomMenu.draw(23, ofGetHeight()/2 + 20 + cameraMenu.getHeight()/3*2 * 2, cameraMenu.getWidth()/2, cameraMenu.getHeight()/3*2);
            ofPushStyle();
            ofSetColor(0, 0, 0);
        }
        ofDrawBitmapString("Orbit", 33, ofGetHeight()/2 + 34 + cameraMenu.getHeight()/3*2 * 2);
        ofPopStyle();
    }
    ofPopStyle();
}

void ofApp::drawHeaders() {
    for(int i = 0; i < sizeof(headerMenu)/sizeof(headerMenu[0]); i++) {
        if(headerHoverIndex == i) {
            if(i != 0)
                headerHover[i].draw(ofGetWidth() - 200 + (headerHover[i-1].getWidth())/4*3, 20, headerHover[i].getWidth()/4*3, headerHover[i].getHeight()/4*3);
            else
                headerHover[i].draw(ofGetWidth() - 200, 20, headerHover[i].getWidth()/4*3, headerHover[i].getHeight()/4*3);
        }
        else {
            if(i != 0)
                headerMenu[i].draw(ofGetWidth() - 200 + (headerMenu[i-1].getWidth())/4*3, 20, headerMenu[i].getWidth()/4*3, headerMenu[i].getHeight()/4*3);
            else
                headerMenu[i].draw(ofGetWidth() - 200, 20, headerMenu[i].getWidth()/4*3, headerMenu[i].getHeight()/4*3);
        }
    }
    
    switch(headerHoverIndex) {
        case 0 : {
            float currentYPos = 20 + (headerMenu[0].getHeight())/4*3;
            for(int i = 0; i < sizeof(subMenuFile)/sizeof(subMenuFile[0]); i++) {
                if(subFileHoverIndex==i) {
                    subFileHover[i].draw(ofGetWidth() - 200, currentYPos, subFileHover[i].getWidth()/4*3, subFileHover[i].getHeight()/4*3);
                    currentYPos += subFileHover[i].getHeight()/4*3;
                }
                else {
                    subMenuFile[i].draw(ofGetWidth() - 200, currentYPos, subMenuFile[i].getWidth()/4*3, subMenuFile[i].getHeight()/4*3);
                    currentYPos += subMenuFile[i].getHeight()/4*3;
                }
            }
            break;
        }
        case 1 : {
            float currentYPos = 20 + (headerMenu[0].getHeight())/4*3;
            for(int i = 0; i < sizeof(subMenuModeling)/sizeof(subMenuModeling[0]); i++) {
                if(subModelingHoverIndex==i) {
                    subModelingHover[i].draw(ofGetWidth() - 200, currentYPos, subModelingHover[i].getWidth()/4*3, subModelingHover[i].getHeight()/4*3);
                    currentYPos += subModelingHover[i].getHeight()/4*3;
                }
                else {
                    subMenuModeling[i].draw(ofGetWidth() - 200, currentYPos, subMenuModeling[i].getWidth()/4*3, subMenuModeling[i].getHeight()/4*3);
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
        
        panelGrayBar.draw(90, 590, 400, 10);
        panelGrayBar.draw(90, 620, 400, 10);
        panelGrayBar.draw(90, 650, 400, 10);
        panelGrayBar.draw(90, 680, 400, 10);
        panelGrayBar.draw(90, 710, 400, 10);
        panelGrayBar.draw(90, 740, 400, 10);
        
        float lengthRatio = (xPos + 3000) / 6000;
        float barLength = 400 * lengthRatio;
        panelGreenBar.draw(90, 590, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 585, 20, 20);
        lengthRatio = (yPos + 3000) / 6000;
        barLength = 400 * lengthRatio;
        panelGreenBar.draw(90, 620, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 615, 20, 20);
        lengthRatio = (zPos + 3000) / 6000;
        barLength = 400 * lengthRatio;
        panelGreenBar.draw(90, 650, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 645, 20, 20);
        lengthRatio = (xRotation + 360) / 720;
        barLength = 400 * lengthRatio;
        panelGreenBar.draw(90, 680, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 675, 20, 20);
        lengthRatio = (yRotation + 360) / 720;
        barLength = 400 * lengthRatio;
        panelGreenBar.draw(90, 710, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 705, 20, 20);
        lengthRatio = (zRotation + 360) / 720;
        barLength = 400 * lengthRatio;
        panelGreenBar.draw(90, 740, barLength, 10);
        panelSliderButton.draw(90 + barLength - 10, 735, 20, 20);
        
        for(int i=0; i<6; i++) {
            if(panelMinusClickIndex==i) {
                panelButtonMinusPressed.draw(500, 587 + 30 * i, 15, 15);
            }
            else {
                panelButtonMinus.draw(500, 587 + 30 * i, 15, 15);
            }
            
            if(panelPlusClickIndex==i) {
                panelButtonPlusPressed.draw(500 + 20, 587 + 30 * i, 15, 15);
            }
            else {
                panelButtonPlus.draw(500 + 20, 587 + 30 * i, 15, 15);
            }
        }
        
        //panelWindow.draw(660, ofGetHeight() - 200, 400, 190);
        string str = "current selected projector : ";
        str += ofToString(currentSelectedProjector);
        str += "\n";
        
        str += "distance from modeling : ";
        str += ofToString(abs(modelPivotPoint.z - projectors[currentSelectedProjector]->zPos) / 100);
        str += "M\n";
        
        if(projectors[currentSelectedProjector]->xPos < modelPivotPoint.x) {
            str += "left distance from pivot point : ";
            str += ofToString(abs(modelPivotPoint.x - projectors[currentSelectedProjector]->xPos) / 100);
            str += "M\n";
        }
        else if(projectors[currentSelectedProjector]->xPos > modelPivotPoint.x) {
            str += "right distance from pivot point : ";
            str += ofToString(abs(modelPivotPoint.x - projectors[currentSelectedProjector]->xPos) / 100);
            str += "M\n";
        }
        
        str += "upper distance from ground : ";
        str += ofToString(projectors[currentSelectedProjector]->yPos / 100);
        str += "M\n";
        ofDrawBitmapString(str, 630, 625);
    }
    else if(isModelingSelected==1) {
        panelWindow.draw(10, ofGetHeight() - 200, ofGetWidth() - 20, 190);
        
        ofDrawBitmapString("Modeling Width : ", ofGetWidth() - 350, ofGetHeight() - 150);
        ofDrawBitmapString("Modeling Height : ", ofGetWidth() - 350, ofGetHeight() - 120);
        
        ofDrawBitmapString(ofToString(modelWidth), ofGetWidth() - 200, ofGetHeight() - 150);
        ofDrawBitmapString(ofToString(modelHeight), ofGetWidth() - 200, ofGetHeight() - 120);
        
        float currentScale = model.getScale().x;
        float lengthRatio = (currentScale) / 50;
        float barLength = 400 * lengthRatio;
        ofDrawBitmapString("Scale Factor", 20, ofGetHeight() - 85);
        ofDrawBitmapString(ofToString(currentScale), 575, ofGetHeight() - 85);
        
        panelGrayBar.draw(120, ofGetHeight() - 95, 400, 10);
        panelGreenBar.draw(120, ofGetHeight() - 95, barLength, 10);
        panelSliderButton.draw(120 + barLength - 10, ofGetHeight() - 100, 20, 20);
        
        if(panelMinusClickIndex==6) {
            panelButtonMinusPressed.draw(530, ofGetHeight() - 150 + 50, 15, 15);
        }
        else {
            panelButtonMinus.draw(530, ofGetHeight() - 150 + 50, 15, 15);
        }
        
        if(panelPlusClickIndex==6) {
            panelButtonPlusPressed.draw(530 + 20, ofGetHeight() - 150 + 50, 15, 15);
        }
        else {
            panelButtonPlus.draw(530 + 20, ofGetHeight() - 150 + 50, 15, 15);
        }
    }
}

void ofApp::clearProject() {
    for(int i=0; i<projectors.size(); i++) {
        if(projectors[i]->isSetted)
            projectors[i]->deactivate();
    }
    //projectors.clear();
    
    meshes.clear();
    model.clear();
    isModelingSelected = -1;
    currentSelectedProjector = -1;
    mappingGUI->projector = NULL;
}

void ofApp::save() {
    ofFile textFile;
    ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + ".txt", "Save your file");
    
    if(saveFileResult.bSuccess) {
        textFile.open(saveFileResult.getPath(), ofFile::WriteOnly);
        if(currentModelURL.compare("")==0)
            textFile << "no modeling" << endl;
        else {
            textFile << currentModelURL << endl;
            textFile << model.getScale().x << endl;
        }
        for(int i=0; i<projectors.size(); i++) {
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
    int itr = 0;
    ofFile textFile;
    ofBuffer buffer;
    vector<string> saveList;
    string str;
    ofFileDialogResult loadFileResult = ofSystemLoadDialog("load file");
    if(loadFileResult.bSuccess) {
        clearProject();
        textFile.open(loadFileResult.getPath(), ofFile::ReadOnly);
        buffer = textFile.readToBuffer();
        for(auto line : buffer.getLines())
            saveList.push_back(line);
        
        itr++;
        
        if(saveList[0].compare("no modeling") != 0) {
            open3DFile(saveList[0]);
            model.setScale(stof(saveList[1]), stof(saveList[1]), stof(saveList[1]));
            reconstructMesh();
            itr++;
        }
        
        for(int i=itr; i<saveList.size()-1; i++) {
            str = ofSplitString(saveList[i], ",")[0];
            int index = stoi(str);
            //ofLog() << str;
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
        if(distance < 75) {
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

void ofApp::closeApp(ofEventArgs &args) {
    exit();
}

void ofApp::dropDownMoveEventCheck(int x, int y) {
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
}

void ofApp::dropDownClickEventCheck(int x, int y, int button) {
    if(button != OF_MOUSE_BUTTON_LEFT)
        return ;
    
    if(isGridDropDownOn) {
        switch(gridDropDownHoverIndex) {
            case 0 : {
                ofVec3f pt = findNearGridPoint(ofPoint(x,y));
                addProjector(pt.x, pt.y, pt.z);
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
                    //easyCam.disableMouseInput();
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
}

void ofApp::cameraMoveEventCheck(int x, int y) {
    if(isCameraMenuClicked) {
        float currentYPos = ofGetHeight()/2 + 20;
        if(x > 23 && x <= 23 + cameraMenu.getWidth()/2) {
            for(int i=0; i<3; i++) {
                cameraModeHoverIndex = -1;
                if(y > currentYPos && y <= currentYPos + cameraMenu.getHeight()/3*2) {
                    cameraModeHoverIndex = i;
                    break;
                }
                currentYPos += cameraMenu.getHeight()/3*2;
            }
        }
        else
            cameraModeHoverIndex = -1;
    }
}

void ofApp::cameraClickEventCheck(int x, int y, int button) {
    if(button != OF_MOUSE_BUTTON_LEFT)
        return ;
    
    //3/4 version
    if(y > (ofGetHeight()/2 - 200)/4*3 && y <= (ofGetHeight()/2 - 200 + cameraZoomBackground.getHeight())/4*3) {
        isCameraMenuClicked = false;
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
        isCameraMenuClicked = false;
        if(y > (ofGetHeight()/2 + 45)/4*3 && y <= (ofGetHeight()/2 + 45 + cameraButton[2].getHeight())/4*3) {
            if(x > 28/4*3 && x <= (28 + cameraButton[2].getWidth())/4*3)
                cameraButtonPressedIndex = 2;
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
    else if(y > ofGetHeight()/2 && y <= (ofGetHeight()/2 + cameraMenu.getHeight()/3*2)) {
        if(x > 23 && x <= 23 + cameraMenu.getWidth()/2) {
            isCameraMenuClicked = true;
        }
    }
    else {
        if(isCameraMenuClicked) {
            isCameraMenuClicked = false;
            if(cameraModeHoverIndex != -1) {
                currentCameraModeIndex = cameraModeHoverIndex;
                cameraModeHoverIndex = -1;
            }
        }
    }
}

void ofApp::panelClickEventCheck(int x, int y, int button) {
    if(button != OF_MOUSE_BUTTON_LEFT)
        return ;
    
    if(currentSelectedProjector != -1 && panelClickIndex == -1) {
        if(y > 590 && y <= 600) {
            float xPos = projectors[currentSelectedProjector]->xPos;
            float lengthRatio = (xPos + 3000) / 6000;
            float barLength = 400 * lengthRatio;
            if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                panelClickIndex = 0;
                //easyCam.disableMouseInput();
            }
        }
        else if(y > 620 && y <= 630) {
            float yPos = projectors[currentSelectedProjector]->yPos;
            float lengthRatio = (yPos + 3000) / 6000;
            float barLength = 400 * lengthRatio;
            if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                panelClickIndex = 1;
                //easyCam.disableMouseInput();
            }
        }
        else if(y > 650 && y <= 660) {
            float zPos = projectors[currentSelectedProjector]->zPos;
            float lengthRatio = (zPos + 3000) / 6000;
            float barLength = 400 * lengthRatio;
            if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                panelClickIndex = 2;
                //easyCam.disableMouseInput();
            }
        }
        else if(y > 680 && y <= 690) {
            float xRotation = projectors[currentSelectedProjector]->xRotation;
            float lengthRatio = (xRotation + 360) / 720;
            float barLength = 400 * lengthRatio;
            if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                panelClickIndex = 3;
                //easyCam.disableMouseInput();
            }
        }
        else if(y > 710 && y <= 720) {
            float yRotation = projectors[currentSelectedProjector]->yRotation;
            float lengthRatio = (yRotation + 360) / 720;
            float barLength = 400 * lengthRatio;
            if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                panelClickIndex = 4;
                //easyCam.disableMouseInput();
            }
        }
        else if(y > 740 && y <= 750) {
            float zRotation = projectors[currentSelectedProjector]->zRotation;
            float lengthRatio = (zRotation + 360) / 720;
            float barLength = 400 * lengthRatio;
            if(x > 90 + barLength - 10 && x <= 90 + barLength + 10) {
                panelClickIndex = 5;
                //easyCam.disableMouseInput();
            }
        }
        
        for(int i=0; i<6; i++) {
            if(y > 587 + 30 * i && y <= 587 + 30 * i + 15) {
                if(x > 500 && x <= 500 + 15)
                    panelMinusClickIndex = i;
                else if(x > 500 + 20 && x <= 500 + 20 + 15)
                    panelPlusClickIndex = i;
            }
        }
    }
    else if(isModelingSelected == 1) {
        if(y > ofGetHeight() - 95 && y <= ofGetHeight() - 85) {
            float currentScale = model.getScale().x;
            float lengthRatio = currentScale / 50;
            float barLength = 400 * lengthRatio;
            if(x > 120 + barLength - 10 && x <= 120 + barLength + 10) {
                panelClickIndex = 6;
                //easyCam.disableMouseInput();
            }
        }
        
        if(y > ofGetHeight() - 150 + 50 && y <= ofGetHeight() - 150 + 50 + 15) {
            if(x > 530 && x <= 530 + 15)
                panelMinusClickIndex = 6;
            else if(x > 530 + 20 && x <= 530 + 20 + 15)
                panelPlusClickIndex = 6;
        }
        
    }
}

void ofApp::panelDragEventCheck(int x, int y, int button) {
    if(currentSelectedProjector != -1 && button == OF_MOUSE_BUTTON_LEFT) {
        if(x >= 90 && x <= 90 + 400) {
            if(panelClickIndex == 0) {
                float realValue = (float)(x - 90) / 400 * 6000 - 3000;
                projectors[currentSelectedProjector]->xPos = floor(realValue);
            }
            else if(panelClickIndex == 1) {
                float realValue = (float)(x - 90) / 400 * 6000 - 3000;
                projectors[currentSelectedProjector]->yPos = floor(realValue);
            }
            else if(panelClickIndex == 2) {
                float realValue = (float)(x - 90) / 400 * 6000 - 3000;
                projectors[currentSelectedProjector]->zPos = floor(realValue);
            }
            else if(panelClickIndex == 3) {
                float realValue = (float)(x - 90) / 400 * 720 - 360;
                projectors[currentSelectedProjector]->xRotation = floor(realValue);
            }
            else if(panelClickIndex == 4) {
                float realValue = (float)(x - 90) / 400 * 720 - 360;
                projectors[currentSelectedProjector]->yRotation = floor(realValue);
            }
            else if(panelClickIndex == 5) {
                float realValue = (float)(x - 90) / 400 * 720 - 360;
                projectors[currentSelectedProjector]->zRotation = floor(realValue);
            }
        }
    }
    else if(isModelingSelected && panelClickIndex == 6 && button == OF_MOUSE_BUTTON_LEFT) {
        if(x >= 120 && x <= 120 + 400) {
            float realValue = (float)(x - 120) / 400 * 50;
            float offset = realValue - model.getScale().x;
            scaleModeling(offset);
        }
    }
}

void ofApp::panelReleaseEventCheck(int x, int y, int button) {
    if(panelClickIndex != -1)
        panelClickIndex = -1;
    if(panelPlusClickIndex != -1) {
        switch(panelPlusClickIndex) {
            case 0 : {
                if(projectors[currentSelectedProjector]->xPos < 3000)
                    projectors[currentSelectedProjector]->xPos++;
                break;
            }
            case 1 : {
                if(projectors[currentSelectedProjector]->yPos < 3000)
                    projectors[currentSelectedProjector]->yPos++;
                break;
            }
            case 2 : {
                if(projectors[currentSelectedProjector]->zPos < 3000)
                    projectors[currentSelectedProjector]->zPos++;
                break;
            }
            case 3 : {
                if(projectors[currentSelectedProjector]->xRotation < 360)
                    projectors[currentSelectedProjector]->xRotation++;
                break;
            }
            case 4 : {
                if(projectors[currentSelectedProjector]->yRotation < 360)
                    projectors[currentSelectedProjector]->yRotation++;
                break;
            }
            case 5 : {
                if(projectors[currentSelectedProjector]->zRotation < 360)
                    projectors[currentSelectedProjector]->zRotation++;
                break;
            }
            case 6 : {
                if(model.getScale().x < 49.99)
                    scaleModeling(0.01);
                break;
            }
        }
        panelPlusClickIndex = -1;
    }
    if(panelMinusClickIndex != -1) {
        switch(panelMinusClickIndex) {
            case 0 : {
                if(projectors[currentSelectedProjector]->xPos > -3000)
                    projectors[currentSelectedProjector]->xPos--;
                break;
            }
            case 1 : {
                if(projectors[currentSelectedProjector]->yPos > -3000)
                    projectors[currentSelectedProjector]->yPos--;
                break;
            }
            case 2 : {
                if(projectors[currentSelectedProjector]->zPos > -3000)
                    projectors[currentSelectedProjector]->zPos--;
                break;
            }
            case 3 : {
                if(projectors[currentSelectedProjector]->xRotation > -360)
                    projectors[currentSelectedProjector]->xRotation--;
                break;
            }
            case 4 : {
                if(projectors[currentSelectedProjector]->yRotation > -360)
                    projectors[currentSelectedProjector]->yRotation--;
                break;
            }
            case 5 : {
                if(projectors[currentSelectedProjector]->zRotation > -360)
                    projectors[currentSelectedProjector]->zRotation--;
                break;
            }
            case 6 : {
                if(model.getScale().x > 0.01)
                    scaleModeling(-0.01);
                break;
            }
        }
        panelMinusClickIndex = -1;
    }
}

void ofApp::headerMoveEventCheck(int x, int y) {
    switch(headerHoverIndex) {
            //resized resource version (original size to 3/4)
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

void ofApp::headerClickEventCheck(int x, int y, int button) {
    if(button != OF_MOUSE_BUTTON_LEFT)
        return ;
    
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
}

void ofApp::subClickEventCheck(int x, int y, int button) {
    if(button != OF_MOUSE_BUTTON_LEFT)
        return ;
    
    switch(subFileHoverIndex) {
        case 0 : {
            //ofLog() << "menu 1";
            clearProject();
            break;
        }
        case 1 : {
            //ofLog() << "menu 2";
            //easyCam.disableMouseInput();
            save();
            break;
        }
        case 2 : {
            //ofLog() << "menu 3";
            //easyCam.disableMouseInput();
            load();
            break;
        }
            //subFileHoverIndex = -1;
    }
    subFileHoverIndex = -1;
    
    switch(subModelingHoverIndex) {
        case 0 : {
            //ofLog() << "menu 1";
            //easyCam.disableMouseInput();
            ofFileDialogResult openFileResult = ofSystemLoadDialog("Select 3d file");
            if(openFileResult.bSuccess)
                open3DFile(openFileResult);
            subModelingHoverIndex = -1;
            break;
        }
    }
}

void ofApp::labelClickEventCheck(int x, int y, int button) {
    if(button != OF_MOUSE_BUTTON_LEFT)
        return ;
    
    if(y > ofGetHeight() - 200 && y <= ofGetHeight() - 200 + 18) {
        int index = 0;
        for(int i=0; i<projectors.size(); i++) {
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
}

void ofApp::objectClickEventCheck(int x, int y, int button) {
    if(button == OF_MOUSE_BUTTON_LEFT) {
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
                currentSelectedProjector = -1;
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
            if(isModelingSelected == 1) {
                if(currentSelectedProjector != -1)
                    projectors[currentSelectedProjector]->isSelected = false;
                currentSelectedProjector = -1;
                mappingGUI->projector = NULL;
                isGridDropDownOn = false;
            }
        }
    }
}
