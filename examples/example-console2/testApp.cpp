#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	/*
	The "panel" is a frame. This frame contains the displayed rectangles, and the scroll bar.
	The scroll bar contains a "grip". The user can drag the grip with the mouse.
	*/

	rectangleWidth = 200;   // Dimensions of displayed rectangles
	rectangleHeight = 150;
	gap = 10;               // Distance between rectangles, and between rectangles and scroll bar
	margin = 20;            // Distance between the edge of the screen and the panel frame
	scrollBarWidth = 20;

	// Now two rectangles, for the scroll bar and his grip placements
	// Coordinates are relative to the panel coordinates, not to the screen coordinates
	// This is a first initialisation, but we don't know many things about these placements at this state
	scrollBarRectangle = ofRectangle(0, 0, scrollBarWidth, 0);
	gripRectangle = ofRectangle(0, 0, scrollBarWidth, 0);

	isDraggingGrip = false; // true when the user is moving the grip

	/*
	Now initialisations for the displayed rectangles
	*/
	int numRectangles = 33;
	colors.resize(numRectangles);      // Each rectangle has got a color, for this example.
	rectangles.resize(numRectangles);  // Position for each rectangle. First rectangle is at x=0, y=0

	for (int i = 0; i < numRectangles; ++i) {
		colors[i].set(ofRandom(0, 255), 0, 0);
		rectangles[i].set(0, 0, rectangleWidth, rectangleHeight);
	}

	ofBackground(127);


}

//--------------------------------------------------------------
void testApp::update(){

	// The size of the panel. All the screen except margins
	panelWidth = ofGetWidth() - margin * 2;
	panelHeight = ofGetHeight() - margin * 2;

	// Space available for displayed rectangles
	int availableWidth = panelWidth - scrollBarWidth - gap;

	// Coordinates for first rectangle
	int x = 0;
	int y = 0;

	ofRectangle * r;

	// Place the rectangles in rows and columns. A row must be smaller than availableWidth
	// After this loop, we know that the rectangles fits the panel width. But the available panel height can be to small to display them all.
	for (int i = 0; i < colors.size(); ++i) {

		r = &rectangles[i];
		r->x = x;
		r->y = y;

		// Now compute the next rectangle position
		x += rectangleWidth + gap;
		if (x + rectangleWidth > availableWidth) {
			scrollBarRectangle.x = r->getRight() + gap; // Adjust the scroll bar position to draw it just at the right
			x = 0;
			y += rectangleHeight + gap;
		}

	}

	gripRectangle.x = scrollBarRectangle.x; // Also adjust the grip x coordinate

	int contentHeight = r->getBottom(); // Total height for all the rectangles
	// TODO: take care if colors.size() == 0

	if (contentHeight > panelHeight) {
		/* In the case where there's not enough room to display all the rectangles */

		/* First, the scroll bar */

			// Show the scroll bar
			isScrollBarVisible = true;
			// Set the scroll bar height to fit the panel height
			scrollBarRectangle.height = panelHeight;


		/* Now, the grip */

			// This ratio is between 0 and 1. The smaller it is, the smaller the grip must be.
			// If its value is 0.5, for example, it means that there's only half of the room in the panel to display all the rectangles.
			float gripSizeRatio = (float)panelHeight / (float)contentHeight;

			// Compute the height of the grip, according to this ratio
			gripRectangle.height = panelHeight * gripSizeRatio;

		/* Now, the vertical scrolling to add to the rectangles position */

			// this ratio, between 0 and 1, tell us the amount of scrolling to add if the grip is at the bottom of the scroll bar
			float scrollMaxRatio = 1 - gripSizeRatio;

			// this ration tell us how much the grip is down. If 0, the grip is at the top of the scroll bar.
			// if 1, the grip is at the bottom of the scroll bar
			float gripYRatio = gripRectangle.y / (scrollBarRectangle.height - gripRectangle.height);

			// Now, the amount of scrolling to do, according to the twos previous ratios
			float contentScrollRatio = gripYRatio * scrollMaxRatio;

			// And now the scrolling value to add to each rectangle y coordinate
			contentScrollY = contentHeight * contentScrollRatio;

	} else {
		/* In the case where there's enough room to display all the rectangles */

		isScrollBarVisible = false;
		contentScrollY = 0;

	}

}

//--------------------------------------------------------------
void testApp::draw(){

	/* First draw the rectangles */

	// Add a translation to bring the panel to the good position
	ofPushMatrix();
	ofTranslate(margin, margin, 0);

	ofRectangle r;

	for (int i = 0; i < colors.size(); ++i) {

		r = rectangles[i];        // the rectangle position in the panel
		r.y -= contentScrollY;    // adjust this position according to the scrolling

		if (r.y < 0) {
			if (r.getBottom() > 0) {
				// Exception 1: If a rectangle is cut at the top of the panel, don't display it, but indicate it's here with a shadow box
				ofSetColor(110);
				ofRect(r.x, 0, r.width, rectangleHeight + r.y);
			}
		} else if (r.getBottom() > panelHeight) {
			if (r.y < panelHeight) {
				// Exception 2: If a rectangle is cut at the bottom of the panel, don't display it, but indicate it's here with a shadow box
				ofSetColor(110);
				ofRect(r.x, r.y, r.width, panelHeight - r.y);
			}
		} else {
			// Draw a rectangle in the panel
			ofSetColor(colors[i]);
			ofRect(r);
		}

	}

	/* Draw the scroll bar, is needed */

	if (isScrollBarVisible) {
		ofSetColor(110);
		ofRect(scrollBarRectangle);
		ofSetColor(180);
		ofRect(gripRectangle);
	}

	// Remove the translation added at the begining
	ofPopMatrix();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

	if (isScrollBarVisible && isDraggingGrip) {

		// Move the grip according to the mouse displacement
		int dy = y - mousePreviousY;
		mousePreviousY = y;
		gripRectangle.y += dy;

		// Check if the grip is still in the scroll bar
		if (gripRectangle.y < 0) {
			gripRectangle.y = 0;
		}
		if (gripRectangle.getBottom() > scrollBarRectangle.getBottom()) {
			gripRectangle.y = scrollBarRectangle.getBottom() - gripRectangle.height;
		}

	}

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	// Check if the click occur on the grip

	if (isScrollBarVisible) {
		ofRectangle r = gripRectangle;
		r.translate(margin, margin); // This translation because the coordinates of the grip are relative to the panel, but the mouse position is relative to the screen
		if (r.inside(x, y)) {
			isDraggingGrip = true;
			mousePreviousY = y;
		}
	}

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	isDraggingGrip = false;

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

	// Place the grip at the top of the scroll bar if the size of the panel change
	gripRectangle.y = 0;

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}