#include "ofxWindowConsole.h" 

namespace gabu {

	namespace log {


		ofxWindowConsole::ofxWindowConsole() {


		}

		ofxWindowConsole& ofxWindowConsole::instance() {

			static ofxWindowConsole instance;
			return instance;

		}


		// some day hopefully we will have all this nicely configuration driven
		void ofxWindowConsole::setup() {

			/* 
			Console logging text formatting/coloring setting 
			*/
			max_num_lines = 1024;

			ofTrueTypeFont::setGlobalDpi(72);
			verdana14.loadFont("verdana.ttf", 14, true, true);
			verdana14.setLineHeight(18.0f);
			verdana14.setLetterSpacing(1.037);

			colors[CTEXT_MESSAGE] = ofColor::black; //(0, 0, 0); // black
			colors[CTEXT_COMMANDLINE] = ofColor::blue; //(0, 0, 1); // blue
			colors[CTEXT_ERROR] = ofColor::red; // (1, 0, 0); // red
			colors[CTEXT_APPMSG] = ofColor::green; // (0, 1, 0); // green
			colors[CTEXT_WARN] = ofColor::bisque; //(0, 1, 1); // something else  - TODO by color names

			/*
			The "panel" is a frame. This frame contains the displayed rectangles, and the scroll bar.
			The scroll bar contains a "grip". The user can drag the grip with the mouse.
			*/

			scrollBarWidth = 20;
			rectangleWidth = ofGetWidth() - scrollBarWidth;   // Dimensions of displayed rectangles
			rectangleHeight = 14;
			gap = 0;               // Distance between rectangles, and between rectangles and scroll bar
			margin = 0;            // Distance between the edge of the screen and the panel frame


			// Now two rectangles, for the scroll bar and his grip placements
			// Coordinates are relative to the panel coordinates, not to the screen coordinates
			// This is a first initialisation, but we don't know many things about these placements at this state
			scrollBarRectangle = ofRectangle(0, 0, scrollBarWidth, 0);
			gripRectangle = ofRectangle(0, 0, scrollBarWidth, 0);

			isDraggingGrip = false; // true when the user is moving the grip

			/*
			Now initialisations for the displayed rectangles
			*/
			int numRectangles = max_num_lines; 
			rectangles.reserve(numRectangles);  // Position for each rectangle. First rectangle is at x=0, y=0

			for (int i = 0; i < numRectangles; ++i) {
				rectangles.push_back(ofRectangle(0, 0, rectangleWidth, rectangleHeight));
				//rectangles[i].set(0, 0, rectangleWidth, rectangleHeight);
			}

			ofBackground(127);

			setToggleKey('+');
			bActive = false;
			enableKeyEvents();


			//test value
			nextLineTime = ofGetElapsedTimeMillis();
		}

		void ofxWindowConsole::printText(const ConsoleText& cText_) {

			clines.push_back(cText_);

		}

		void ofxWindowConsole::mouseDragged(ofMouseEventArgs& data) { //int x, int y, int button) {

			if (isScrollBarVisible && isDraggingGrip) {

				// Move the grip according to the mouse displacement
				int dy = data.y - mousePreviousY;
				mousePreviousY = data.y;
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

		void ofxWindowConsole::mousePressed(ofMouseEventArgs& data) { //int x, int y, int button) {

			// Check if the click occur on the grip

			if (isScrollBarVisible) {
				ofRectangle r = gripRectangle;
				r.translate(margin, margin); // This translation because the coordinates of the grip are relative to the panel, but the mouse position is relative to the screen
				if (r.inside(data.x, data.y)) {
					isDraggingGrip = true;
					mousePreviousY = data.y;
				}
			}

		}

		void ofxWindowConsole::mouseReleased(ofMouseEventArgs& data) { //int x, int y, int button) {
			isDraggingGrip = false;

		}

		void ofxWindowConsole::windowResized(int w, int h) {
			// Place the grip at the top of the scroll bar if the size of the panel change
			gripRectangle.y = 0;

		}

		void ofxWindowConsole::update(ofEventArgs &data) {
			static int count = 0;
			float time = ofGetElapsedTimeMillis() - nextLineTime;
			if(time > 1000) {
				nextLineTime = ofGetElapsedTimeMillis();
				//printText(ConsoleText(CTEXT_MESSAGE, "This is a random test: "+ofToString(count++)));
			}

			/*
			Lets check if we have more than expected log lines in our buffer. Purge from the end
			*/

			if(clines.size() > max_num_lines)
				clines.resize(max_num_lines); // not sure if this is the best way to trim the capacity. If this does reallocation, not good. TODO.


			// The size of the panel. All the screen except margins
			panelWidth  = ofGetWidth()  - margin * 2;
			panelHeight = ofGetHeight()/2 - margin * 2;

			// Space available for displayed rectangles
			int availableWidth = panelWidth - scrollBarWidth - gap;

			// Coordinates for first rectangle
			// TODO: where do we anchor the console window?
			int x = 0;
			int y = ofGetHeight()/2; //0;

			ofRectangle * r = 0;
			scrollBarRectangle.x = rectangleWidth + gap;

			// Place the rectangles in rows and columns. A row must be smaller than availableWidth
			// After this loop, we know that the rectangles fits the panel width. But the available panel height can be to small to display them all.
			for (int i = 0; i < clines.size(); ++i) {

				r = &rectangles[i];
				r->x = x;
				r->y = y;
				y += rectangleHeight + gap;

			}

			gripRectangle.x = scrollBarRectangle.x; // Also adjust the grip x coordinate

			int contentHeight = 0;
			if(r)
				contentHeight = r->getBottom(); // Total height for all the rectangles
			// TODO: take care if r.size() == 0

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

		void ofxWindowConsole::draw(ofEventArgs &data) {

			if(!bActive) return;
			/* First draw the rectangles */

			// Add a translation to bring the panel to the good position
			ofPushMatrix();
			ofTranslate(margin, margin, 0);
			glColor3f(255, 255, 255);
			ofRect(0, 0, ofGetWidth(), ofGetHeight()/2.0);
			
			ofRectangle r;

			if(clines.size() == 0) return;

			for (int i = 0; i < clines.size(); ++i) {
			//for (int i = clines.size()-1; i>= 0; i--) {

				r = rectangles[i];        // the rectangle position in the panel
				r.y -= contentScrollY;    // adjust this position according to the scrolling

				if (r.y < 0) {
					if (r.getBottom() > 0) {
						// Exception 1: If a rectangle is cut at the top of the panel, don't display it, but indicate it's here with a shadow box
						ofSetColor(110);
						//ofRect(r.x, 0, r.width, rectangleHeight + r.y);
						verdana14.drawString(clines[i].text, r.x, 0);
					}
				} else if (r.getBottom() > panelHeight) {
					if (r.y < panelHeight) {
						// Exception 2: If a rectangle is cut at the bottom of the panel, don't display it, but indicate it's here with a shadow box
						ofSetColor(110);
						//ofRect(r.x, r.y, r.width, panelHeight - r.y);
						verdana14.drawString(clines[i].text, r.x, r.y);
					}
				} else {
					// Draw a rectangle in the panel
					ofSetColor(colors[clines[i].type]);
					verdana14.drawString(clines[i].text, r.x, r.y);
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



		void ofxWindowConsole::exit(ofEventArgs &data) {

		}

		void ofxWindowConsole::clearConsole() {
			clines.clear(); // TODO - we dont want to trim memory capacity. Just the content. Verify.
		}

		void ofxWindowConsole::setBackgroundColor(ofColor& c_) {

		}

		void ofxWindowConsole::setTextColor(ofColor& c_) {
			colors[CTEXT_MESSAGE] = c_; 
		}

		void ofxWindowConsole::setWarnTextColor(ofColor& c_) {
			colors[CTEXT_WARN] = c_;
		}

		void ofxWindowConsole::setErrorTextColor(ofColor& c_) {
			colors[CTEXT_ERROR] = c_;
		}

		void ofxWindowConsole::setBufferSize(size_t sz_) {
			max_num_lines = sz_;
		}

		void ofxWindowConsole::setToggleKey(int key){
			toggleKey = key;
		}

		//enable OF keyEvents
		// for now clubbing the mouse events too
		void ofxWindowConsole::enableKeyEvents(){
			ofAddListener(ofEvents().keyPressed, this, &ofxWindowConsole::keyPressed);
			ofAddListener(ofEvents().keyReleased, this, &ofxWindowConsole::keyReleased);
			ofAddListener(ofEvents().mouseDragged, this, &ofxWindowConsole::mouseDragged);
			ofAddListener(ofEvents().mouseReleased, this, &ofxWindowConsole::mouseReleased);
			ofAddListener(ofEvents().mousePressed, this, &ofxWindowConsole::mousePressed);

		}

		void ofxWindowConsole::disableKeyEvents(){
			ofRemoveListener(ofEvents().keyPressed, this, &ofxWindowConsole::keyPressed);
			ofRemoveListener(ofEvents().keyReleased, this, &ofxWindowConsole::keyReleased);
			ofAddListener(ofEvents().mouseDragged, this, &ofxWindowConsole::mouseDragged);
			ofAddListener(ofEvents().mouseReleased, this, &ofxWindowConsole::mouseReleased);
			ofAddListener(ofEvents().mousePressed, this, &ofxWindowConsole::mousePressed);
		}

		void ofxWindowConsole::keyPressed(ofKeyEventArgs &e) {
			int key = e.key;
			if(key==toggleKey) {
				toggleActive();
				return;
			}
			if(!bActive) return;
		}

		void ofxWindowConsole::keyReleased(ofKeyEventArgs &e) {
		}

		//activate (show) deactivate (hide) console
		void ofxWindowConsole::toggleActive(){
			bActive = !bActive;
		}

	}
}