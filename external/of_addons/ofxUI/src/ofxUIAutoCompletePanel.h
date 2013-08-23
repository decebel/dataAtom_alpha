#ifndef OFXUI_AUTOCOMPLETEPANEL_H
#define OFXUI_AUTOCOMPLETEPANEL_H

#include "ofxUIDropDownList.h"
#include "ofxUI.h"
#include <boost/shared_ptr.hpp>


class ofxUIDropDownList2 : public ofxUIToggle
{
public:    
	ofxUIDropDownList2(string _name, vector<string> items, float w = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_MEDIUM) : ofxUIToggle()
	{
		init(_name, items, w, x, y, _size);
	}

	// DON'T USE THE NEXT CONSTRUCTORS
	// This is maintained for backward compatibility and will be removed on future releases

	ofxUIDropDownList2(float x, float y, float w, string _name, vector<string> items, int _size) : ofxUIToggle()
	{
		init(_name, items, w, x, y, _size);
		//        ofLogWarning("OFXUIDROPDOWNLIST: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");        
	}

	ofxUIDropDownList2(float w, string _name, vector<string> items, int _size) : ofxUIToggle()
	{
		init(_name, items, w, 0, 0, _size);
		//        ofLogWarning("OFXUIDROPDOWNLIST: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");        
	}

	ofxUIDropDownList2(float x, float y, string _name, vector<string> items, int _size) : ofxUIToggle()
	{
		init(_name, items, 0, x, y, _size);
		//        ofLogWarning("OFXUIDROPDOWNLIST: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");        
	}

	//    ofxUIDropDownList(string _name, vector<string> items, int _size)
	//    {
	//        init(_name, items, 0, 0, 0, _size);
	//        ofLogWarning("OFXUIDROPDOWNLIST: DON'T USE THIS CONSTRUCTOR. THIS WILL BE REMOVED ON FUTURE RELEASES.");        
	//    }    

	void init(string _name, vector<string> items, float w = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_MEDIUM)
	{

		//_textInput = new ofxUITextInput("cc", "textstring", 300, _size); //320, 20); //, x, y, _size);
		rect = new ofxUIRectangle(x,y,w,0);
		autoSize = w == 0 ? true : false;
		name = string(_name);  		        
		kind = OFX_UI_WIDGET_DROPDOWNLIST2; 		        
		paddedRect = new ofxUIRectangle(-padding, -padding, padding*2.0, padding*2.0);
		paddedRect->setParent(rect); 


		//_textInput->setRectParent(rect);
		size = _size; 
		label = new ofxUILabel(0,0,(name+" LABEL"), name, _size); 
		label->setParent(label); 
		label->setRectParent(rect);
		label->setEmbedded(true);

		value = new bool(); 
		*value = false; 
		draw_fill = *value;

		allowMultiple = false; 
		initToggles(items, _size);         
		autoClose = false; 
		singleSelected = NULL; 

		selected_index = -1;

	}

	virtual void draw()
	{
		ofPushStyle();

		ofEnableBlendMode(OF_BLENDMODE_ALPHA);

		drawPadded();
		drawPaddedOutline();

		drawBack();

		drawOutline();
		drawOutlineHighlight();

		drawFill();
		drawFillHighlight();

		ofPopStyle();
	}

	void clearToggles()
	{        
		while(toggles.size())
		{
			ofxUILabelToggle *t = toggles[0]; 			
			removeToggle(t->getName());
		}
	}    

	void clearSelected()
	{
		for(int i = 0; i < toggles.size(); i++)
		{
			toggles[i]->setValue(false);
		}            
		selected.clear();
	}

	void addToggle(string toggleName)
	{        
		float yt = rect->getHeight();

		for(int i = 0; i < toggles.size(); i++)
		{
			ofxUILabelToggle *t = toggles[i]; 			
			yt +=t->getRect()->getHeight();         
		}	

		ofxUILabelToggle *ltoggle;         
		if(autoSize)
		{
			ltoggle = new ofxUILabelToggle(0, yt, false, toggleName, size);                 
		}
		else
		{
			ltoggle = new ofxUILabelToggle(0, yt, rect->getWidth(), rect->getHeight(), false, toggleName, size);                 
		}
		ltoggle->getRect()->setParent(this->getRect());
		ltoggle->getRect()->y = rect->y+yt; 			        
		ltoggle->getRect()->x = rect->x; 			        
		ltoggle->setVisible(*value); 
		ltoggle->setLabelVisible(*value);             
		toggles.push_back(ltoggle);        
		parent->addWidget(ltoggle);
		ltoggle->setParent(this);
		ltoggle->setModal(modal);        
		if(isOpen())
		{
			open(); 
		}           
		else
		{
			close();
		}
	}    

	void removeToggle(string toggleName)
	{
		ofxUILabelToggle *t = NULL; 
		for(int i = 0; i < toggles.size(); i++)
		{
			ofxUILabelToggle *other = (ofxUILabelToggle *)toggles[i];
			if(other->getName() == toggleName)
			{
				t = other;
				toggles.erase(toggles.begin()+i);                                             
				break; 
			}
		}
		for(int i = 0; i < selected.size(); i++)
		{
			ofxUILabelToggle *other = (ofxUILabelToggle *)selected[i];
			if(other->getName() == toggleName)
			{
				selected.erase(selected.begin()+i);                                             
				break; 
			}
		}                
		if(t != NULL)
		{
			parent->removeWidget(t);

			float yt = rect->getHeight();
			for(int i = 0; i < toggles.size(); i++)
			{
				ofxUILabelToggle *t = toggles[i]; 			
				t->setParent(this); 
				t->getRect()->setParent(this->getRect());                 
				t->getRect()->y = yt; 
				t->getRect()->x = 0; 
				yt +=t->getRect()->getHeight();         
			}		            

		}

	}

	vector<ofxUIWidget *> & getSelected()
	{                 
		return selected;         
	}

	int selectedIndex() {
		return selected_index;
	}


	void initToggles(vector<string> &items, int _size)
	{
		float ty = 20;
		for(int i = 0; i < items.size(); i++)
		{
			string tname = items[i]; 
			ofxUILabelToggle *ltoggle; 
			if(autoSize)
			{
				ltoggle = new ofxUILabelToggle(0, ty, false, tname, _size, true);                 
			}
			else
			{
				ltoggle = new ofxUILabelToggle(0, ty, rect->getWidth(), false, tname, _size, true);                 
			}
			ltoggle->setVisible(*value); 
			ltoggle->setLabelVisible(*value);             
			toggles.push_back(ltoggle); 
			ty+=20; 
		}        
	}

	void setLabelText(string labeltext)
	{
		label->setLabel(labeltext);
		if(!autoSize)
		{
			ofxUIRectangle *labelrect = label->getRect();
			float h = labelrect->getHeight();
			float ph = rect->getHeight();
			float w = labelrect->getWidth();
			float pw = rect->getWidth();
			labelrect->y = (int)(ph*.5 - h*.5);
			labelrect->x = (int)(pw*.5 - w*.5-padding*.5);
		}
	}

	void setParent(ofxUIWidget *_parent)
	{
		parent = _parent;         
		rect->height = label->getPaddingRect()->height+padding*2.0;
		ofxUIRectangle *labelrect = label->getRect(); 
		if(autoSize)
		{
			rect->width = label->getPaddingRect()->width+padding*2.0;             
		}
		else
		{
			while(labelrect->width+padding*4.0 > rect->width)
			{
				string labelstring = label->getLabel();
				string::iterator it;
				it=labelstring.end();
				it--; 
				labelstring.erase (it); 
				label->setLabel(labelstring);
			}             
		}

		float h = labelrect->getHeight(); 
		float ph = rect->getHeight();
		float w = labelrect->getWidth(); 
		float pw = rect->getWidth(); 

		labelrect->y = (int)(ph*.5 - h*.5);
		labelrect->x = (int)(pw*.5 - w*.5-padding*.5); 
		paddedRect->height = rect->height+padding*2.0;  
		paddedRect->width = rect->width+padding*2.0;          

		float yt = rect->height;
		for(int i = 0; i < toggles.size(); i++)
		{
			ofxUILabelToggle *t = toggles[i]; 			
			t->setParent(this); 
			t->getRect()->setParent(this->getRect()); 
			t->getRect()->x = 0;
			t->getRect()->y = yt; 			
			yt +=t->getRect()->getHeight();         
			if(autoSize)
			{
				t->getRect()->setWidth(rect->getWidth());
			}
			t->getPaddingRect()->setWidth(paddedRect->getWidth());
		}
	}	    

	void mouseReleased(int x, int y, int button) 
	{
		if(rect->inside(x, y) && hit)
		{
			setValue(!(*value));
#ifdef TARGET_OPENGLES
			state = OFX_UI_STATE_NORMAL;        
#else            
			state = OFX_UI_STATE_OVER; 
#endif 
			triggerEvent(this); 
		}    
		else
		{
			state = OFX_UI_STATE_NORMAL;         
		}
		stateChange();     
		hit = false; 
	}

	void setAutoClose(bool _autoClose)
	{
		autoClose = _autoClose; 
	}

	void open()
	{
		setValue(true);
	}

	void close()
	{
		setValue(false);
	}


	void setVisible(bool _visible)
	{
		visible = _visible; 
		label->setVisible(visible); 
		for(int i = 0; i < toggles.size(); i++)
		{
			ofxUILabelToggle * toggle = (ofxUILabelToggle *) toggles[i];
			toggle->setVisible((visible && isOpen()));
		}
	}

	void setToggleVisibility(bool _value)
	{

		//getTextInput()->setVisible(_value);

		for(int i = 0; i < toggles.size(); i++)
		{
			ofxUILabelToggle * toggle = (ofxUILabelToggle *) toggles[i];
			toggle->setVisible(_value); 
			toggle->setLabelVisible(_value); 
		}
	}

	vector<ofxUILabelToggle *> &getToggles()
	{
		return toggles; 
	}

	//ofxUITextInput* getTextInput() {
	//	return _textInput;
	//}

	void triggerSelf()
	{
		if(parent != NULL)
		{
			parent->triggerEvent(singleSelected); 
		}        
	}

	void triggerEvent(ofxUIWidget *child)
	{        
		if(child == this)
		{
			parent->triggerEvent(child); 
			return; 
		}

		if(autoClose)
		{
			if(isOpen())
			{
				close();
			}
		}

		if(!allowMultiple)
		{
			activateToggle(child->getName().c_str()); 
		}

		selected.clear();
		for(int i = 0; i < toggles.size(); i++)
		{
			ofxUILabelToggle *t = toggles[i]; 		
			if(t->getValue())
			{
				selected.push_back(t);
			}                           
		}        


		if(parent != NULL)
		{
			parent->triggerEvent(this);
			parent->triggerEvent(child); 
		}        
	}	    

	void activateToggle(int index) {

		if(index >=0 && index < toggles.size()) {

			for(int i=0; i<toggles.size(); i++) {
				ofxUILabelToggle *t = toggles[i];
				if(i == index) {				
					t->setValue(true); 		
					singleSelected = t; 				
					selected_index = index;
				}
				else {
					t->setValue(false); 
				}
			}
		} else if (index >= toggles.size() || index < 0) {

			ofxUILabelToggle *t = toggles[0];
			t->setValue(true); 		
			singleSelected = t; 
			
			selected_index = 0;

			for(int i=1; i<toggles.size(); i++) {
				t->setValue(false); 
			}
		}
	}

	void activateToggle(string _name)
	{
		for(int i = 0; i < toggles.size(); i++)
		{
			ofxUILabelToggle *t = toggles[i];
			//printf("\nC=%s.",t->getName().c_str());
			if(!(t->getName().compare(_name.c_str())))
			{
				printf("\nC=%s.",t->getName().c_str());
				t->setValue(true); 		
				singleSelected = t; 

				//DECE
				selected_index = i;
			}
			else 
			{
				t->setValue(false); 
			}			
		}
	}    

	void setAllowMultiple(bool _allowMultiple)
	{
		allowMultiple = _allowMultiple; 
	}

	virtual void setValue(bool _value)
	{
		*value = _value;
		draw_fill = *value;
		setModal(*value);
		setToggleVisibility(*value);
		label->setDrawBack((*value));        
	}

	virtual void setModal(bool _modal)      //allows for piping mouse/touch input to widgets that are outside of parent's rect/canvas
	{
		modal = _modal;
		if(parent != NULL)
		{
			if(modal)
			{
				parent->addModalWidget(this);
			}
			else
			{
				parent->removeModalWidget(this);
			}
		}

		for(int i = 0; i < toggles.size(); i++)
		{
			toggles[i]->setModal(modal);
		}
	}    

	bool isOpen()
	{
		return *value;
	}

protected:    //inherited: ofxUIRectangle *rect; ofxUIWidget *parent; 
	bool autoSize; 
	bool autoClose; 
	vector<ofxUILabelToggle *> toggles; 
	ofxUILabelToggle *singleSelected; 
	vector<ofxUIWidget *> selected; 
	bool allowMultiple; 
	int size;


	//SPofxUITextInput _sptextInput;
	ofxUITextInput* _textInput;
	int selected_index;
}; 



//class ofxUIAutoCompletePanel : public ofxUIWidget {
//	
//	typedef boost::shared_ptr<ofxUIDropDownList> SPofxUIDropDownList; 
//	SPofxUIDropDownList _spdropDownUIList;
//
//	typedef boost::shared_ptr<ofxUITextInput> SPofxUITextInput;
//	SPofxUITextInput _sptextInput;
//
//public:
//
//	ofxUIAutoCompletePanel(float w, string _name, vector<string> items, int _size) 
//	{
//	  _sptextInput.reset(new ofxUITextInput(_name, "textstring", w, 0, 0, 0, _size));
//	  _spdropDownUIList.reset(new ofxUIDropDownList(w, _name, items, _size));
//	}
//
//
//};

#endif

