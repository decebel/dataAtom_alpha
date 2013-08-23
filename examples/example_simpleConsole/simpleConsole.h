//#ifndef SIMPLECONSOLE_H
//#define SIMPLECONSOLE_H
//
//#include "ofxConsoleBase.h"
//#include "ofMain.h"
//
//class simpleConsole : public ofxConsoleBase {
//	
//public:
//	simpleConsole(){
//		enableKeyEvents();
//		bActive = false;
//		//setDefaultCommand(this, &ofxConsole::defaultFunction);
//		
//		setBackgroundColor(0, 0, 0);
//		setTextColor(160, 160, 160);
//		setCommandlineColor(40, 40, 40);
//		setCommandlineTextColor(255, 230, 0);
//		setMessageTextColor(255, 255, 255);
//		setErrorTextColor(255, 0, 0);
//		setToggleKey('+');
//	}
//	
//	virtual ~simpleConsole(){
//	    disableKeyEvents();
//	}
//	void defaultFunction(const std::vector<std::string> & args){
//		print("< "+args[0]+" > is not a registered command!", CTEXT_ERROR);
//	}
//	
//	//close non boost way
//	void close(const std::vector<std::string> & args){
//		bActive = false;
// 	}
//	
//	//close boost way
//	void close(){
//		bActive = false;
// 	}
//	
//	void setToggleKey(int key){
//		toggleKey = key;
//	}
//	
//	//enable OF keyEvents
//	void enableKeyEvents(){
//		ofAddListener(ofEvents().keyPressed, this, &simpleConsole::keyPressed);
//		ofAddListener(ofEvents().keyReleased, this, &simpleConsole::keyReleased);
//	}
//	
//	void disableKeyEvents(){
//		ofRemoveListener(ofEvents().keyPressed, this, &simpleConsole::keyPressed);
//		ofRemoveListener(ofEvents().keyReleased, this, &simpleConsole::keyReleased);
//	}
//	
//	//set text collor rgb(0,255);
//	void setTextColor(int r, int g, int b){
//		copyTextColor.r = r/255.0;
//	    copyTextColor.g = g/255.0;
//		copyTextColor.b = b/255.0;
//	}
//	
//	//set background collor rgb(0,255);
//	void setBackgroundColor(int r, int g, int b){
//		backgroundColor.r = r/255.0;
//	    backgroundColor.g = g/255.0;
//		backgroundColor.b = b/255.0;
//	}
//	
//	//set commandline background color rgb(0,255);
//	void setCommandlineColor(int r, int g, int b){
//		commandlineColor.r = r/255.0;
//	    commandlineColor.g = g/255.0;
//		commandlineColor.b = b/255.0;
//	}
//	
//	//set commandline text color rgb(0,255);
//	void setCommandlineTextColor(int r, int g, int b){
//		commandlineTextColor.r = r/255.0;
//	    commandlineTextColor.g = g/255.0;
//		commandlineTextColor.b = b/255.0;
//	}
//	
//	//set message text color rgb(0,255);
//	void setMessageTextColor(int r, int g, int b){
//		messageTextColor.r = r/255.0;
//	    messageTextColor.g = g/255.0;
//		messageTextColor.b = b/255.0;
//	}
//	
//	//set error text color rgb(0,255);
//	void setErrorTextColor(int r, int g, int b){
//		errorTextColor.r = r/255.0;
//	    errorTextColor.g = g/255.0;
//		errorTextColor.b = b/255.0;
//	}
//	
//	//activate (show) deactivate (hide) console
//	void toggleActive(){
//		bActive = !bActive;
//	}
//	
//	//customText
//	void drawBitmapString(const std::string str, float x, float y, void *font){
//
//		ofDrawBitmapString(str, x, y);
//		//const char * cstr = str.c_str();
//		//int len, i;
//		//glRasterPos2f(x, y);
//		//len = (int) strlen(cstr);
//		//for (i = 0; i < len; i++)
//		//{
//		//	glutBitmapCharacter(font, cstr[i]);
//		//}
//		
//	}
//	
//	void drawBitmapString(const std::string str, float x, float y){
//
//		ofDrawBitmapString(str, x, y);
//
//		//const char * cstr = str.c_str();
//		//int len, i;
//		//glRasterPos2f(x, y);
//		//len = (int) strlen(cstr);
//		//for (i = 0; i < len; i++)
//		//{
//		//	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, cstr[i]);
//		//}
//		
//	}
//	
//	
//
//	//simple render function
//	virtual void render(){
//		if(!bActive) return;
//		glColor3f(backgroundColor.r, backgroundColor.g, backgroundColor.b);
//		ofRect(0, 0, ofGetWidth(), ofGetHeight()/2.0);
//		glColor3f(commandlineColor.r, commandlineColor.g, commandlineColor.b);
//		ofRect(0, ofGetHeight()/2.0-20, ofGetWidth(), 20);
//		std::list<consoleText>::iterator iter;
//		int i = 1;
//		for(iter = textBuffer.begin(); iter!=textBuffer.end(); iter++){
//			consoleText str = *(iter);
//			str.insert(0, ">> ");
//			switch(str.textType){
//				case CTEXT_MESSAGE:
//					glColor3f(messageTextColor.r, messageTextColor.g, messageTextColor.b);
//					drawBitmapString(str, 20, ofGetHeight()/2.0-30-15*(textBuffer.size()-i));
//					break;
//				case CTEXT_COPY:
//					glColor3f(copyTextColor.r, copyTextColor.g, copyTextColor.b);
//					drawBitmapString(str, 20, ofGetHeight()/2.0-30-15*(textBuffer.size()-i));
//					break;
//				case CTEXT_ERROR:
//					glColor3f(errorTextColor.r, errorTextColor.g, errorTextColor.b);
//					drawBitmapString(str, 20, ofGetHeight()/2.0-30-15*(textBuffer.size()-i));
//					break;
//			}
//			i++;
//		}
//		glColor3f(commandlineTextColor.r, commandlineTextColor.g, commandlineTextColor.b);
//		drawBitmapString(commandLine, 20, ofGetHeight()/2.0-6);
//		
//	}
//	
//	//process key events
//	void keyPressed(ofKeyEventArgs &e){
//		int key = e.key;
//		if(key==toggleKey) {
//			toggleActive();
//			return;
//		}
//		if(!bActive) return;
//		switch(key){
//			case OF_KEY_BACKSPACE:
//				passBackspace();
//				break;
//			case OF_KEY_RETURN:
//				passIntro();
//				break;
//			case OF_KEY_UP:
//                historyGo(-1);
//				break;
//			case OF_KEY_DOWN:
//                historyGo(1);
//				break;
//			default:
//				passKey(key);
//				break;
//		}
//		
//	}
//	void keyReleased(ofKeyEventArgs &e){}
//	
//protected:
//	//settings
//	bool bActive;
//	int toggleKey;
//	ofColor copyTextColor, messageTextColor, commandlineTextColor, errorTextColor;
//	ofColor commandlineColor, backgroundColor;
//};
//
//
//#endif