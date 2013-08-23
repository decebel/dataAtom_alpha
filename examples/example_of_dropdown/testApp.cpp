#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup()
{
    ofBackground(50);
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 320; 
    gui = new ofxUICanvas(length+xInit+2, 0, length+xInit, ofGetHeight()); 
    
    //gui->addWidgetDown(new ofxUILabel("DYNAMIC DROP DOWN LIST", OFX_UI_FONT_LARGE)); 
	t = gui->addTextInput("commander", "", length);
    //gui->addWidgetDown(new ofxUILabel("PRESS '1' TO ADD TO LIST", OFX_UI_FONT_MEDIUM)); 
    //gui->addWidgetDown(new ofxUILabel("PRESS '2' TO DELETE FROM LIST", OFX_UI_FONT_MEDIUM));     
    //gui->addWidgetDown(new ofxUILabel("PRESS '3' TO DELETE ALL IN LIST", OFX_UI_FONT_MEDIUM));         
    vector<string> names; 
    names.push_back("HELP: ");    
	names.push_back("FB: ");    names.push_back("TW: ");    names.push_back("CME: ");    names.push_back("BTEC: ");    names.push_back("TWEB: ");
    ddl = new ofxUIDropDownList2(length-xInit, "TYPE HELP OR F1 ANYTIME", names, OFX_UI_FONT_MEDIUM);


    //gui->addTextInput("COMMAND PALETTE", "Input Text", length-xInit)->setAutoClear(false);
	//gui->setWidgetFontSize(OFX_UI_FONT_MEDIUM);

    gui->addWidgetDown(ddl);
    gui->setTheme(OFX_UI_THEME_VEGAN);
    gui->autoSizeToFitWidgets(); 
//    gui->setDrawWidgetPadding(true);
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
	ofAddListener(t->newCommandEvent, this, &testApp::commandEvent);
}


//--------------------------------------------------------------
void testApp::update()
{

}

//--------------------------------------------------------------
void testApp::draw()
{

}
//--------------------------------------------------------------
void testApp::exit()
{
    delete gui; 
}

void testApp::commandEvent(ofxUIEventArgs &e)
{
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
		//else if(!ddl->isOpen() && t->getTextString().empty()) {
		//	ddl->open();
		//}

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

	//ddl->activateToggle(t->getTextString());
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName(); 
    
    cout << "WIDGET NAME: " << name << endl; 
    
    if(name == "FULLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());   
    }
    else if(name == "DYNAMIC DROP DOWN")
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
        for(int i = 0; i < selected.size(); i++)
        {
            cout << "SELECTED: " << selected[i]->getName() << endl; 
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    switch (key) {
        case 'p':
            gui->setDrawWidgetPadding(true);
            break;
        case 'P':
            gui->setDrawWidgetPadding(false);
            break;
            
        case '1':
            ddl->addToggle("NEW TOGGLE");
            break;

        case '2':
            ddl->removeToggle("NEW TOGGLE");
            break;

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

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}