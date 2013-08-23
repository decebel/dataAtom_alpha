#pragma once

#include <vector>
#include <deque>
#include "ofMain.h"
#include "ConsoleLoggingUtil.h"

namespace gabu {

	namespace log {

		class ofxWindowConsole {

		private:	            

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
			ofTrueTypeFont	verdana14;
	
			deque<ConsoleText> clines;
			ofColor colors[5]; // TODO - hash the color codes based on text types
			int max_num_lines;
			int toggleKey;
			bool bActive;
			float nextLineTime; // test code
			ofxWindowConsole(); 
			ofxWindowConsole(const ofxWindowConsole&);
			ofxWindowConsole& operator = (const ofxWindowConsole&);
		public:

			// ideally we have only one console window. For now a simple singleton. 
			//maybe an object on the heap is better. we have control over recycling, hooking up events and we can watch for 
			//resources when we cleanup the console. Internally, we can return a singleton. TODO. 
			static ofxWindowConsole& instance(); 
			

			void printText(const ConsoleText& cText_);
			void mouseDragged(ofMouseEventArgs& data) ; //int x, int y, int button);
			void mousePressed(ofMouseEventArgs& data); //int x, int y, int button);
			void mouseReleased(ofMouseEventArgs& data); //int x, int y, int button);
			void windowResized(int w, int h);

			void setup();
			void update(ofEventArgs &data);
			void draw(ofEventArgs &data);
			void exit(ofEventArgs &data);

			// for run-time customization - like from commandline
			void clearConsole();
			void setBackgroundColor(ofColor& c_);
			void setTextColor(ofColor& c_);
			void setWarnTextColor(ofColor& c_);
			void setErrorTextColor(ofColor& c_);
			void setBufferSize(size_t sz_);


			void setToggleKey(int k);
			void enableKeyEvents();
			void disableKeyEvents();
			void keyPressed(ofKeyEventArgs &e);
			void keyReleased(ofKeyEventArgs &e);
			void toggleActive();


		};
	}

}