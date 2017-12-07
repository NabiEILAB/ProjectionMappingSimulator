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
        void exit();
		
    //User defined variables
    public :
        //Easy Camera
        ofEasyCam easyCam;
    
        //GUI panel variables (currently, replaced ofPanel with custom resource panel)
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
        ofPoint modelPivotPoint;
    
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
        ofImage cameraMenu;
        ofImage cameraMenuClicked;
        ofImage cameraTopMenu;
        ofImage cameraTopMenuHover;
        ofImage cameraCenterMenu;
        ofImage cameraCenterMenuHover;
        ofImage cameraBottomMenu;
        ofImage cameraBottomMenuHover;
        int cameraButtonPressedIndex;
        int currentCameraModeIndex;
        int cameraModeHoverIndex;
        bool isCameraMenuClicked;
    
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
        ofImage panelButtonPlus;
        ofImage panelButtonPlusPressed;
        ofImage panelButtonMinus;
        ofImage panelButtonMinusPressed;
        int headerHoverIndex;
        int subFileHoverIndex;
        int subModelingHoverIndex;
        int panelClickIndex;
        int panelPlusClickIndex;
        int panelMinusClickIndex;
    
        int currentSelectedProjector;
        int isModelingSelected;
        string currentModelURL;
        bool exiting;
    
    //User defined functions
    public :
        //void refreshGUI();
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
        void clearProject();
        void save();
        void load();
        ofVec3f findNearGridPoint(ofPoint mousePt);
        int findNearProjectorIndex(ofPoint mousePt);
        void scaleModeling(float factor);
        void closeApp(ofEventArgs& args);
        void dropDownMoveEventCheck(int x, int y);
        void dropDownClickEventCheck(int x, int y, int button);
        void cameraMoveEventCheck(int x, int y);
        void cameraClickEventCheck(int x, int y, int button);
        void panelClickEventCheck(int x, int y, int button);
        void panelDragEventCheck(int x, int y, int button);
        void panelReleaseEventCheck(int x, int y, int button);
        void headerMoveEventCheck(int x, int y);
        void headerClickEventCheck(int x, int y, int button);
        void subClickEventCheck(int x, int y, int button);
        void labelClickEventCheck(int x, int y, int button);
        void objectClickEventCheck(int x, int y, int button);
};
