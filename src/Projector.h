//
//  Projector.h
//  ProjectorSimulator
//
//  Created by Nabi Lab on 2017. 7. 17..
//
//

#ifndef Projector_h
#define Projector_h

#include "ofMain.h"
#include "ofxOpenCv.h"

class Projector {
public :
    ofParameterGroup projectorParameters;
    ofParameter<float> xPos;
    ofParameter<float> yPos;
    ofParameter<float> zPos;
    ofParameter<float> xRotation;
    ofParameter<float> yRotation;
    ofParameter<float> zRotation;
    
    //Radiation value of projector's light. - this is a debug value
    float xRadVal = 1;
    float yRadVal = 1.6;
    float zRadVal = 1;
    
    int projectorNum;
    bool isSetted;
    bool isSelected;
    bool isMappingOn;
    
    float width,height;
    float pivotWidth, pivotHeight, pivotDistance;
    
    ofVideoPlayer videoPlayer;
    
    //Used with mappingApp
    ofTexture copiedTexture;
    ofPoint pt[4];
    
    string currentVideoURL;
    
    //Projector's Matrices
    ofMatrix4x4 projectorProjection; //Projection (Frustrum) Matrix
    ofMatrix4x4 projectorView;       //View (Look at) Matrix
    ofMatrix4x4 projectorBias;       //Bias (0.5 scale, 0.5 translate) Matrix
    ofMatrix4x4 projectorMatrix;     //Bias * Projection * View Matrix
    
    //Projector's FBO (FBO contains shadow map and video scene as texture)
    ofFbo shadowFbo;
    ofFbo textureFbo;
    
    Projector();
    Projector(float xPos, float yPos, float zPos);
    Projector(float xPos, float yPos, float zPos, float xRotation, float yRotation, float zRotation);
    Projector(float xPos, float yPos, float zPos, float xRotation, float yRotation, float zRotation, int projectorNum);
    void setup();
    void update();
    void draw();
    void allocateShadowFbo();
    void allocateTextureFbo(int width, int height);
    void clearShadowFbo();
    void clearTextureFbo();
    void activate(float xPos, float yPos, float zPos);
    void deactivate();
};


#endif /* Projector_h */
