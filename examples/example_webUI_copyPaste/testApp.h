#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include <Awesomium/WebCore.h>


class ofxUIScrollableCanvas;
class ofxTexturedUI;
class ofxConsole;

namespace minto {
	namespace UI {
	class PluginDashboardUIManager;
	}
}

class testApp : public ofBaseApp {

public:
	testApp();
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void gotMessage(ofMessage msg);
	void dragEvent(ofDragInfo dragInfo);
	
private:
	Awesomium::WebView* webView;
	Awesomium::WebCore* webCore;
	ofImage _img;
	bool isResizing;
	int webTexWidth;
	int webTexHeight;
	ofTexture webTex;

	ofPolyline _line;
	bool trackUserText;
	int mouseX;
	int mouseY;
	int oldMouseX;
	int oldMouseY;

	void guiEvent(ofxUIEventArgs &e);
	ofxUIScrollableCanvas *gui4;
	//ofxUICanvas *gui4;
	ofxTexturedUI* webTexture;
	void setGUI();
	bool isLoadingComplete;

	void setupWebEngine();
	void webUpdate();
	void webEngineDraw();
	ofxConsole* console;
	void setupConsole();

	minto::UI::PluginDashboardUIManager* plugUIManager;
	void setupPlugUIManager();
	float delayedRun;
};
