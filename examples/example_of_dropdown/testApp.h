#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxUIAutoCompletePanel.h"

class testApp : public ofBaseApp{

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    ofxUICanvas *gui; 
    ofxUIDropDownList2 *ddl; 
    void exit(); 
    void guiEvent(ofxUIEventArgs &e);
	void commandEvent(ofxUIEventArgs &e);
    ofxUITextInput* t; 
    
};
