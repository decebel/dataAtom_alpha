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

		vector<ofImage> images;  // Object to draw in the panel.
		int imagesWidth;
		int imagesHeight;

		/* Display parameters for the panel */
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
		bool isMouseOverGrip;
		int mousePreviousY;
		vector<ofRectangle> rectangles;

};
