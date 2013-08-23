#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    gui = new ofxUICanvas();
	setgui4();

    gui->addLabel("OFXUI TUTORIAL", OFX_UI_FONT_LARGE);
    gui->addSpacer(); 
    gui->addSlider("BACKGROUND VALUE",0.0,255.0,100.0); 
    gui->addToggle("FULLSCREEN", false);
    gui->autoSizeToFitWidgets(); 
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent); 
    gui->loadSettings("GUI/guiSettings.xml");
    
}

void testApp::setgui4() {

	float dim = 16; 


	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit; 
	gui4 = new ofxUIScrollableCanvas(length*3+xInit*3+6, 0, length+xInit, ofGetHeight());     
	gui4->setSnapping(true);
    gui4->addWidgetDown(new ofxUILabel("PANEL 4: SCROLLABLE", OFX_UI_FONT_LARGE)); 	
    gui4->addSpacer(length-xInit, 2);

	unsigned char pixels[200*100*3];
	
	for(int i=0; i<200*100*3; i++) {
		pixels[i] = (int)(255 * ofRandomf());
	}
	
	texture.allocate(200, 100, GL_RGB);
	texture.loadData(pixels, 200, 100, GL_RGB);

	webTexture = new ofxTexturedUI(length-xInit, 0, 100, 100, NULL, "test");
	gui4->addWidgetDown(webTexture);
	//webTexture->setTexture(&texture);

    //gui4->addWidgetDown(new ofxUILabel("BILABEL SLIDER", OFX_UI_FONT_MEDIUM)); 				
    //gui4->addWidgetDown(new ofxUIBiLabelSlider(length-xInit, 0, 100, 50, "BILABEL", "HOT", "COLD", OFX_UI_FONT_MEDIUM));

    //gui4->addWidgetDown(new ofxUILabel("MINIMAL SLIDER", OFX_UI_FONT_MEDIUM)); 				
    //gui4->addWidgetDown(new ofxUIMinimalSlider(length-xInit, dim, 0, 100, 50.0, "MINIMAL",OFX_UI_FONT_MEDIUM));

    //gui4->addSpacer(length-xInit, 2);
    //
    //gui4->addWidgetDown(new ofxUILabel("CIRCLE SLIDER", OFX_UI_FONT_MEDIUM)); 				
    //gui4->addWidgetDown(new ofxUICircleSlider((length-xInit)*.5, 0, 100, 50.0, "NORTH SOUTH", OFX_UI_FONT_MEDIUM));    
    //gui4->addSpacer(length-xInit, 2);
    //gui4->addWidgetDown(new ofxUILabel("FPS SLIDER", OFX_UI_FONT_MEDIUM)); 				
    //gui4->addFPSSlider("FPS SLIDER", length-xInit, dim);

    //vector<float> buffer; 
    //for(int i = 0; i < 256; i++)
    //{
    //    buffer.push_back(0.0);
    //}

    //gui4->addWidgetDown(new ofxUILabel("MOVING GRAPH", OFX_UI_FONT_MEDIUM)); 				    
    //mg = (ofxUIMovingGraph *) gui4->addWidgetDown(new ofxUIMovingGraph(length-xInit, 120, buffer, 256, 0, 400, "MOVING GRAPH"));
    
    //gui4->addSpacer(length-xInit, 2);
    //gui4->addWidgetDown(new ofxUILabel("IMAGE SAMPLER", OFX_UI_FONT_MEDIUM)); 				
    ////gui4->addWidgetDown(new ofxUIImageSampler(img->getWidth(), img->getHeight(), img, "SAMPLER"));
    //gui4->addWidgetDown(new ofxUIMultiImageButton(dim*2, dim*2, false, "GUI/toggle.png", "IMAGE BUTTON"));
    //gui4->addWidgetDown(new ofxUIMultiImageToggle(dim*2, dim*2, false, "GUI/toggle.png", "IMAGE BUTTON"));
	ofAddListener(gui4->newGUIEvent,this,&testApp::guiEvent);
}


//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	//texture.draw(0, 0);
}
//--------------------------------------------------------------
void testApp::exit()
{
    gui->saveSettings("GUI/guiSettings.xml"); 
    delete gui; 
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "BACKGROUND VALUE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        ofBackground(slider->getScaledValue());
    }
    else if(e.widget->getName() == "FULLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());   


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
		case 'w':
			webTexture->setTexture(&texture);
			gui4->addWidgetDown(webTexture);
			webTexture->toggleVisible();
		case 'W':
			webTexture->toggleVisible();
            
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