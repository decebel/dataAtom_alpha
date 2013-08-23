#include "testApp.h"
#include "clipBoardWin.h"
#include <Awesomium\BitmapSurface.h>
//#include "ScriptEngineBridger.h"
#include "ScriptEngineMain.h"
#include "ofxConsole.h"
#include "ui\dashboard\WindowConsoleLogger.h"
#include "ui\dashboard\PluginDashboardUIManager.h"

using namespace Awesomium;

testApp::testApp() : webCore(0), webView(0), console(0), plugUIManager(0) {}



void testApp::setupConsole() {

	console = new ofxConsole();

}

void testApp::setupWebEngine() {

	trackUserText = false;
	isResizing = false;
	webTexWidth = 300; //ofGetWindowWidth()/2;
	webTexHeight = ofGetWindowHeight() - 100;
    webTex.allocate(webTexWidth, webTexHeight, GL_RGBA); 

	setGUI();


	    // Disable scrollbars via the WebCoreConfig
	Awesomium::WebConfig config;
    //config.setCustomCSS("::-webkit-scrollbar { display: none; }");
    
	// Create our WebCore singleton with the default options
	webCore = WebCore::Initialize(config);
	
	// Create a new WebView instance with a certain width and height, using the
	// WebCore we just created
	webView = webCore->CreateWebView(webTexWidth, webTexHeight);
	
	// Load a certain URL into our WebView instance
	std::string str_address("http://www.wikipedia.com/");
	WebURL url(WebString::CreateFromUTF8(str_address.c_str(), str_address.length()));
	webView->LoadURL(url);
	isLoadingComplete = false;
	webView->Focus();

}
void testApp::setupPlugUIManager() {
	plugUIManager = new minto::UI::PluginDashboardUIManager();
	plugUIManager->setupUI();
}

void testApp::setup(){

	delayedRun = ofGetElapsedTimeMillis();
	ofEnableSmoothing(); 
	ofBackground(0); 

	setupPlugUIManager();
	//setupConsole();

	gabu::log::WindowConsoleLogger::instance().enableAppEvents();

	//ScriptEngineMain::instance().initializeEngine();
	
	setupWebEngine();
	
	oldMouseX = mouseX = 0;
	oldMouseX = mouseY = 0;
	
}


void testApp::setGUI() {

	float dim = 16; 


	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit; //length*3+xInit*3+6
	gui4 = new ofxUIScrollableCanvas(0, 0, 350, ofGetHeight());         
	gui4->setSnapping(true);
	gui4->setColorFill(ofColor::aquamarine);
    gui4->addWidgetDown(new ofxUILabel("PANEL 4: SCROLLABLE", OFX_UI_FONT_LARGE)); 	
    gui4->addSpacer(length-xInit, 2);
	webTexture = new ofxTexturedUI(webTexWidth, webTexHeight, NULL, "test");

	gui4->addWidgetDown(webTexture);

	std::cout<<"w="<<gui4->getRect()->getWidth()<<" h="<<gui4->getRect()->getHeight()<<std::endl;
	std::cout<<"x="<<webTexture->getRect()->getX()<<" y="<<webTexture->getRect()->getY()<<" w="<<webTexture->getRect()->getWidth()<<" h="<<webTexture->getRect()->getHeight()<<std::endl;


	ofAddListener(gui4->newGUIEvent,this,&testApp::guiEvent);
}


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

void testApp::exit(){
    // Destroy our WebView instance
	if(webView)
		webView->Destroy();
	if(console)
		delete console;
	if(plugUIManager)
		delete plugUIManager;
}

void testApp::webUpdate() {

	webCore->Update();

	BitmapSurface* surface = (BitmapSurface*)webView->surface();		
	if (surface && surface->is_dirty()) {
		webTex.loadData(surface->buffer(), webTexWidth, webTexHeight, GL_BGRA); 			
		if(isLoadingComplete) {
			webTexture->setTexture(&webTex);
			isLoadingComplete = false;
		}

	}
}

void testApp::update(){

	static int once = 0;
	if(!once) {

		ScriptEngineMain::instance().initializeEngine();
		once = 1;
	}
	webUpdate();


	// delayed loading of plugin check

	static bool once2 = false;
	if(!once2) {
	float t = ofGetElapsedTimeMillis() - delayedRun;
	if(t > 3000) {
		t = 0;
		//plugUIManager->onPluginLoaded();
		once2 = true;
	}
	 
	}

}

void testApp::webEngineDraw() {

    ofSetColor(255);

	if (webView->IsLoading()) {
        ofSetColor(0);
        ofDrawBitmapString("Loading...", 10, ofGetHeight()-20);
		isLoadingComplete = false;
    } else {
		isLoadingComplete = true;
	}
	
	
	ofEnableAlphaBlending();
	ofSetColor(ofColor::black, 127);
	ofRect(0, 0, ofGetWindowWidth(), 30);
	ofDisableAlphaBlending();
	ofSetColor(ofColor::white);
	ofDrawBitmapString("Press 'c' for Copy, 'p' for Paste", 50, 20);

}


void testApp::draw(){

	webEngineDraw();

	if(trackUserText) {
		ofSetColor(ofColor::brown);
		_line.draw();
	}

	if(console)
		console->render();

	ofBackground(ofColor::white);//233, 52, 27, 255); 
	ofPushStyle(); 
	ofEnableBlendMode(OF_BLENDMODE_ALPHA); 
	ofPopStyle();



}


void testApp::keyPressed(int key){
	switch (key) {
		case 'c':
			if(webView)
				webView->Copy();
			break;
		case 'v':
			if(webView)
				webView->Paste();
			break;
	}
}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y ){
	if(webView)
		webView->InjectMouseMove(x, y);
	mouseX = x;
	mouseY = y;
	_line.addVertex(x,y);
}


void testApp::mouseDragged(int x, int y, int button){
	if(webView)
		webView->InjectMouseMove(x, y);
}


void testApp::mousePressed(int x, int y, int button){
	if(webView)
		webView->InjectMouseDown(Awesomium::MouseButton::kMouseButton_Left);
	trackUserText = true;
}


void testApp::mouseReleased(int x, int y, int button){
	if(webView) {
		webView->InjectMouseUp(Awesomium::MouseButton::kMouseButton_Left);
		webView->Copy();
	}
	_line.clear();
	std::string data = getClipBoardString();
	std::cout<<"clipboard has :"<<data<<std::endl;
}


void testApp::windowResized(int w, int h){

	isResizing = true;
	//webTexWidth = w;
	//webTexHeight = h;
	//webTex.allocate(w, h, GL_RGBA);

	//webView->Resize(w, h);
	//webCore->Update();

}

void testApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
