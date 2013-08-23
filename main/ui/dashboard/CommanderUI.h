#ifndef COMMANDERUI_H
#define COMMANDERUI_H

#include <string>
#include <vector>

class ofxUICanvas;
class ofxUIDropDownList2;
class ofxUIEventArgs;
class ofxUITextInput;
class ofEventArgs;
class ofKeyEventArgs;

class CommanderTabUI {

protected:
	ofxUICanvas* gui;			// gui to hold the drop down list
	ofxUIDropDownList2* ddl;	// drop down list itself
	ofxUITextInput* t;			// enter command in the text box
	float length;
	float spacing;
	bool bActive;
	int toggleKey;
	std::vector<std::string> commands; 
	
	std::vector<std::string> loadSettings(const std::string& xml_);

public:

	void setup();
	void enableEvents();
	void disableEvents();
	
	void keyPressed(ofKeyEventArgs& e); 
	void setActivateKey(int key);
	
	void commandEvent(ofxUIEventArgs &e);
	void update();
	void draw();
	void exit(ofEventArgs& args);
	void toggleUI();

	// should we pass a registrar interface?
	void wireEventRegistrar();
	void validate_and_fire(const std::string& name, const std::string& command);
	// todo - methods for adding commands + loading commands auto+startup
	// datastructure for sort+filter based on user inputs. suffix tree ? 
	// overlay with history for most used commands






};


#endif