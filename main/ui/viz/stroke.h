#ifndef STROKE_H
#define STROKE_H

#include <ofMain.h>

class stroke {
    
public: 
    
    ofPolyline originalLine;
    ofPolyline animateTo;
    ofPolyline animated;
    
    ofPolyline animatedTarget;
    
    ofPoint newPta, newPtb;
    bool bDone;
    bool bFlip;
    
    void clear();
    void done();    
    void update();    
    void draw();
    
};


#endif