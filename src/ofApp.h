#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "Projector.h"
#include "MappingApp.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    //Additional variables & functions
    public :
        //Easy Camera
        ofEasyCam easyCam;
    
        //GUI panel variables
        ofxPanel gui;
        ofParameterGroup projectorPanelGroup;
    
        //Dynamic array Projector class vector
        vector<Projector*> projectors;
    
        //Model loader
        ofxAssimpModelLoader model;
        ofVboMesh mesh;
    
        //Shader
        ofShader textureProjectionShader;
        ofShader depthStoringShader;
    
        //Window Pointers
        shared_ptr<ofAppBaseWindow> mappingWindow;
        shared_ptr<MappingApp> mappingGUI;
    
        int currentSelectedProjector;
    
        void refreshGUI();
        void addProjector();
        void deleteProjector(int projectorNum);
        void renderCustomModel();
        void renderPrimitiveModel();
        void open3DFile(ofFileDialogResult openFileResult);
        void reconstructMesh();
        void openVideoFile(ofFileDialogResult openFileResult);
        void setProjectorShader(int index);
};
