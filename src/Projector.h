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
    float xRadVal = 2;
    float yRadVal = 1;
    float zRadVal = 1;
    
    int projectorNum;
    bool isSelected;
    bool isTrsMode; //for gizmo mode
    
    ofVideoPlayer videoPlayer;
    
    Projector();
    Projector(float xPos, float yPos, float zPos);
    Projector(float xPos, float yPos, float zPos, float xRotation, float yRotation, float zRotation);
    Projector(float xPos, float yPos, float zPos, float xRotation, float yRotation, float zRotation, int projectorNum);
    void setup();
    void update();
    void draw();
    
};


#endif /* Projector_h */
