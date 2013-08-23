#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);//, 0, 0, 255);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	img.loadImage("indispensable.jpg");
	ofSetLineWidth(2);

	
	for(int i=0; i<200*100*3; i++) {
		pixels[i] = (int)(255 * ofRandomf());
	}

	texture.allocate(200, 100, GL_RGB);
	texture.loadData(pixels, 200, 100, GL_RGB);
	
	ofBackground(233, 52, 27);
	guiSetup();
}


//--------------------------------------------------------------
void testApp::update(){

	for(int i=0; i<200*200*3; i++) {
		pixels[i] = (int)(255 * ofRandomf());
	}

	texture.clear();
	texture.allocate(200, 200, GL_RGB);
	texture.loadData(pixels, 200, 200, GL_RGB);

}

//--------------------------------------------------------------
void testApp::draw(){
		
	//if (ofGetElapsedTimef() < 10) {
	//	ofSetColor(40, 100, 100);
	//	
	//	char fpsStr[255]; // an array of chars
	//	
	//	sprintf(fpsStr, "press [f] to \ntoggle fullscreen\nmove mouse horizontaly\nfor speed");
	//	ofDrawBitmapString(fpsStr, ofGetWidth()/2 - 82,ofGetHeight()/2 - 30);		
	//}
	
	//img.draw(100, 10);

	texture.draw(400, 400);

	//img.drawSubsection(100, 100, 100, 100, 
	// uncomment to see the frame - currently framerate limited to 60fps
	/*
	char fpsStr[255]; // an array of chars	
	sprintf(fpsStr, "frame rate: %f", ofGetFrameRate());
	ofDrawBitmapString(fpsStr, 10,10);
	*/
	

	
	// create screenshot
	//if (bSnapshot == true){
		//screenGrabber.grabScreen(0,0,ofGetWidth(),ofGetHeight());
		//char fileName[255];
		//sprintf(fileName, "snapshot_%3i.png", snapCounter++);
		//screenGrabber.saveImage(fileName);
		//bSnapshot = false;
	//}
}

void testApp::guiSetup() {

	gui = new ofxUICanvas(110, 110, 200, 400);
	std::cout<<"w="<<gui->getRect()->getWidth()<<" h="<<gui->getRect()->getHeight()<<std::endl;
	gui->addWidgetDown(new ofxUILabel("PANEL 1: BASICS", OFX_UI_FONT_LARGE)); 
    //gui->addWidgetDown(new ofxUILabel("Press 'h' to Hide GUIs", OFX_UI_FONT_LARGE)); 
	std::cout<<"w="<<gui->getRect()->getWidth()<<" h="<<gui->getRect()->getHeight()<<std::endl;
	ofxTexturedUI* texturedUI = new ofxTexturedUI(180, 176, &texture, "test");
	texturedUI->setCropImageToFitRect(true);	
	gui->addWidgetDown(texturedUI);
	std::cout<<"w="<<gui->getRect()->getWidth()<<" h="<<gui->getRect()->getHeight()<<std::endl;

	std::cout<<"x="<<texturedUI->getRect()->getX()<<" y="<<texturedUI->getRect()->getY()<<" w="<<texturedUI->getRect()->getWidth()<<" h="<<texturedUI->getRect()->getHeight()<<std::endl;
	ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);    
	

}

void testApp::guiEvent(ofxUIEventArgs &e) {

}


void testApp::exit() {

	delete gui; 

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'f') {
		ofToggleFullscreen();
	}else if(key == 'p') {
		// create a screenshot
		//bSnapshot = true;
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
//	rotationSteps= x / (ofGetWidth() / 2.0);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	//closing =true;
	//opening = false;
	//lastSwitch = ofGetElapsedTimef();

}

//--------------------------------------------------------------
void testApp::windowResized(int windowResized, int h){

}

////--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
