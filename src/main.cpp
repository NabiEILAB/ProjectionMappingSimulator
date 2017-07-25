#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context
    ofGLWindowSettings glSet;
    glSet.width = 1024;
    glSet.height = 768;
    glSet.setGLVersion(3, 2);
    ofCreateWindow(glSet);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
