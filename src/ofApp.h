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
        ofxAssimpModelLoader projectorModel;
        vector<ofMesh> meshes;
        ofMesh projectorMesh;
        float modelWidth;
        float modelHeight;
    
        //Shaders
        ofShader textureProjectionShader;
        ofShader depthStoringShader;
        ofShader projectorTextureShader;
        ofShader modelingBasicShader;
    
        //Window Pointers
        shared_ptr<ofAppBaseWindow> mappingWindow;
        shared_ptr<MappingApp> mappingGUI;
    
        //UI resources
        ofPoint clickedCoord;
    
        ofImage gridDropDown[1];
        ofImage gridDropDownHover[1];
        int gridDropDownHoverIndex;
        bool isGridDropDownOn;
    
        ofImage projectorDropDown[2];
        ofImage projectorDropDownHover[2];
        int projectorDropDownHoverIndex;
        bool isProjectorDropDownOn;
    
        ofImage cameraZoomBackground;
        ofImage cameraMoveBackground;
        ofImage cameraButton[6];
        ofImage cameraButtonPressed[6];
        int cameraButtonPressedIndex;
    
        ofImage headerMenu[2];
        ofImage headerHover[2];
        ofImage subMenuFile[3];
        ofImage subFileHover[3];
        ofImage subMenuModeling[1];
        ofImage subModelingHover[1];
    
        ofImage panelWindow;
        ofImage panelGrayBar;
        ofImage panelGreenBar;
        ofImage panelSliderButton;
        ofImage panelLabel;
        ofImage panelLabelClick;
        int headerHoverIndex;
        int subFileHoverIndex;
        int subModelingHoverIndex;
        int panelClickIndex;
    
        int currentSelectedProjector;
        int isModelingSelected;
        string currentModelURL;
    
        void refreshGUI();
        void addProjector(float x, float y, float z);
        void deleteProjector(int projectorNum);
        void settingCameraPosition();
        void renderCustomModel();
        void renderProjectorModel();
        void open3DFile(ofFileDialogResult openFileResult);
        void open3DFile(string url);
        void reconstructMesh();
        void reconstructProjectorMesh();
        void openVideoFile(ofFileDialogResult openFileResult);
        void setProjectorShader(int index);
        void drawDropDownMenus();
        void drawButtons();
        void drawHeaders();
        void drawPanel();
        void save();
        void load();
        ofVec3f findNearGridPoint(ofPoint mousePt);
        int findNearProjectorIndex(ofPoint mousePt);
        void scaleModeling(float factor);
};
