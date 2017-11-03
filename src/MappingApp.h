//
//  MappingApp.h
//  ProjectorSimulator
//
//  Created by Nabi E.I.Lab on 2017. 10. 12..
//
//

#ifndef MappingApp_h
#define MappingApp_h

#include "ofMain.h"
#include "Projector.h"

class MappingApp : public ofBaseApp {
public :
    Projector* projector;
    ofTexture texture;
   
    ofPoint points[4];
    ofPoint realPoints[4];
    ofPoint centerPt;
    ofPoint realCenterPt;
    int leftX,rightX,upY,downY;
    
    
    //MappingApp(Projector* projector);
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
    void setProjector(Projector* projector);
    void setWarpingMode();
    void toRealValue();
};


#endif /* MappingApp_h */
