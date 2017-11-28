#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"
#include "MappingApp.h"

//========================================================================
int main( ){
	//setup the GL context
    ofGLFWWindowSettings glSet;
    
    glSet.width = 1024;
    glSet.height = 768;
    glSet.setGLVersion(3, 2);
    glSet.setPosition(ofVec2f(300,300));
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(glSet);
    
    glSet.width = 750;
    glSet.height = 768;
    glSet.decorated = false; //removes window border
    glSet.setPosition(ofVec2f(1324,300));
    shared_ptr<ofAppBaseWindow> mappingWindow = ofCreateWindow(glSet);
    

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    
    shared_ptr<ofApp> mainApp(new ofApp);
    shared_ptr<MappingApp> mappingApp(new MappingApp);
    mainApp->mappingWindow = mappingWindow;
    mainApp->mappingGUI = mappingApp;
    
    
    //ofRunApp(new ofApp());
    ofRunApp(mappingWindow, mappingApp);
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
