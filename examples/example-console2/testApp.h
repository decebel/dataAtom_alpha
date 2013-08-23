#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		vector<ofColor> colors;            // Object to draw in the panel. Here just colored rectangles.

		/* Display parameters for the panel */
			int rectangleWidth;
			int rectangleHeight;
			int gap;
			int margin;
			int scrollBarWidth;


		int panelWidth;
		int panelHeight;
		int contentScrollY;
		bool isScrollBarVisible;
		ofRectangle scrollBarRectangle;
		ofRectangle gripRectangle;
		bool isDraggingGrip;
		int mousePreviousY;
		vector<ofRectangle> rectangles;

};
