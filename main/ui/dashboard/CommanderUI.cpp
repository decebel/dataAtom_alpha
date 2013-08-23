#include "CommanderUI.h"
#include "ofMain.h"
#include "ofxUI.h"
#include "ofxUIAutoCompletePanel.h"
#include "extension\EventsRegistrar.h"
#include <boost/algorithm/string.hpp> //TODO: move to string utility
#include <algorithm>

void CommanderTabUI::setup() {
	length = 320;

	spacing = OFX_UI_GLOBAL_WIDGET_SPACING; 
	gui = new ofxUICanvas(length+spacing+2, 0, length+spacing, ofGetHeight()); 
	t = gui->addTextInput("commander", "", length);
	
	commands = loadSettings("commanderUI.xml"); 

	ddl = new ofxUIDropDownList2(length-spacing, "TYPE HELP OR F1 ANYTIME", commands, OFX_UI_FONT_MEDIUM);

	gui->addWidgetDown(ddl);
	gui->setTheme(OFX_UI_THEME_HACKER); // todo - theme to be loaded as well. we should load an object from our xml one time.
	gui->autoSizeToFitWidgets(); 

	setActivateKey('~'); // todo - loaded from config


	wireEventRegistrar();

	enableEvents();
}

void CommanderTabUI::wireEventRegistrar() {

	using namespace minto::ev;

	//EventsRegistrar::instance() ..
}



void CommanderTabUI::enableEvents() {


	ofAddListener(t->newCommandEvent, this, &CommanderTabUI::commandEvent);
	ofAddListener(ofEvents().keyPressed, this, &CommanderTabUI::keyPressed);
	ofAddListener(ofEvents().exit, this, &CommanderTabUI::exit);
}

void CommanderTabUI::disableEvents() {

	ofRemoveListener(t->newCommandEvent, this, &CommanderTabUI::commandEvent);
}

std::vector<std::string> CommanderTabUI::loadSettings(const std::string& xml_) {

	vector<string> names; 
	names.push_back("HELP:");    
	names.push_back("FB:");    
	names.push_back("TW:");    
	names.push_back("CME:");    
	names.push_back("BTEC:");    
	names.push_back("TWEB:");

	return names;	
}

void CommanderTabUI::keyPressed(ofKeyEventArgs& e) {

	if(toggleKey == e.key)  {
		toggleUI();
		return;
	}

	switch (e.key) {
		//case 'p':
		//    gui->setDrawWidgetPadding(true);
		//    break;
		//case 'P':
		//    gui->setDrawWidgetPadding(false);
		//    break;
		//    
		//case '2':
		//    ddl->removeToggle("NEW TOGGLE");
		//    break;

	case '3':
		//ddl->clearToggles();
		ddl->close();
		break;

	case '4':
		//ddl->setLabelText("TEST LABEL");
		ddl->open();
		break;
	case '5':
		t->simulateMouseClick();
		break;
	case '6':
		ddl->close();
		break;

	default:
		break;
	}
}

void CommanderTabUI::setActivateKey(int key) {
	toggleKey = key;

}

void CommanderTabUI::commandEvent(ofxUIEventArgs &e) {

	string name = e.widget->getName(); 
	ofxUITextInput* t = (ofxUITextInput *) e.widget;
	cout << name << ": "<<t->getTextString() <<endl; 

	// we should also get an event when we are ready to close the drop-down. Not sure at the moment

	if(t->getNavigateKey() == OF_KEY_RETURN) {
		if(ddl->isOpen()) 
			ddl->close();
		else if(!ddl->isOpen())
			ddl->open();

		t->simulateMouseClick();

		// TODO: time to execute command 
		validate_and_fire(name, t->getTextString());		
		//t->setAutoClear(true);
	}
	else if(t->getNavigateKey() == OF_KEY_UP) {
		if(ddl->isOpen() && ddl->selectedIndex() == -1) {
			ddl->close();
		} else if(ddl->isOpen()) {
			int index = ddl->selectedIndex();
			if(index == -1) {
				// TODO - index to the last item 
				index = ddl->getToggles().size() - 1;
				ddl->activateToggle(index);
			} else {
				int index = ddl->selectedIndex() - 1;
				ddl->activateToggle(index);
			}

		}

	} else if(t->getNavigateKey() == OF_KEY_DOWN) {
		if(!ddl->isOpen() && t->getTextString().empty()) {
			ddl->open();
		} else if(ddl->isOpen()) {
			int index = ddl->selectedIndex();
			if(index == -1) {
				index = 0;
				ddl->activateToggle(index);
			} else {
				int index = ddl->selectedIndex() + 1;
				ddl->activateToggle(index);
			}
		}

	} else if(!t->getTextString().empty()) {
		if(!ddl->isOpen())
			ddl->open();
		ddl->activateToggle(t->getTextString());
	}


	// event for command history search. 
	// most popular items drop down functionality can be implemented

	

}

void CommanderTabUI::validate_and_fire(const std::string& name, const std::string& command) {

	size_t p = command.find(':');
	if(p == std::string::npos) 
		return;

	//if(command.length() >= 2) {
	

	std::string com = command.substr(0, p+1);
	boost::trim(com);
	boost::to_upper(com);
	std::vector<std::string>::iterator it = std::find(commands.begin(), commands.end(), com);

	if((std::find(commands.begin(), commands.end(), com)) == commands.end()) {
		std::cout<<"Not a valid command: "<<command<<" finding: "<<com<<std::endl;
		return;
	}

	std::cout<<"Valid command: "<<command;

	// find the appropriate command and let the registrar know
	using namespace minto::ev;
	uuid uid(1); // TODO - 
	EventsRegistrar::instance().onCommandText(uid, name, t->getTextString() );

	//}


}

void CommanderTabUI::update() {

}

void CommanderTabUI::draw() {

}

void CommanderTabUI::exit(ofEventArgs& args) {

	// TODO: cleanup in correct order is necessay before we turn on below code. 
	//delete ddl;
	//delete t;
	//delete gui;

}


void CommanderTabUI::toggleUI() {
	bActive = !bActive;

	// until we have better control on drawing, bActive is not of much use
	gui->toggleVisible();
	if(bActive) {
		t->simulateMouseClick();
	}

}
