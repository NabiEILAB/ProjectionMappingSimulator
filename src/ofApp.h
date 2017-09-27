#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "Projector.h"

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
    
        //Primitive object (for the video test)
        ofPlanePrimitive plane;
        ofBoxPrimitive box;
        ofBoxPrimitive box2;
        ofSpherePrimitive ball;
    
        //Dynamic array of3dPrimitive vector
        vector<of3dPrimitive> models;
    
        //Dynamic array ofVec3f vector (for the store multiple model attribute)
        vector<ofVec3f> modelAttributes;
    
        //Shader
        ofShader textureProjectionShader;
        ofShader depthStoringShader;
    
        void refreshGUI();
        void deleteProjector(int projectorNum);
        void renderCustomModel();
        void renderPrimitiveModel();
};
