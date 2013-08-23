#include "PluginDashboardUIManager.h"
#include "scripting\scriptEngineEvents.h"
#include "CommanderUI.h"
#include "ofMain.h"
#include "ofxUI.h"
#include <strstream>


namespace minto {

	namespace UI {

		PluginDashboard::PluginDashboard() {
			dim = 16; // todo - read from config
			lenDim = 255; //same
		}


		void PluginDashboard::setup() {
	
			float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
			float length = lenDim -xInit; 
			pluginsCanvas = new ofxUIScrollableCanvas(length*3+xInit*3+6, 0, length+xInit, ofGetHeight());
			pluginsCanvas->setColorBack(ofColor::gray);
			pluginsCanvas->addWidgetDown(new ofxUILabel("MARKETS/MEDIA", OFX_UI_FONT_LARGE)); 	
			pluginsCanvas->setDrawBack(true);    

			pluginsCanvas->addSpacer(length-xInit, 2);

			//pluginsCanvas->addWidgetDown(new ofxUILabel("BILABEL SLIDER", OFX_UI_FONT_MEDIUM)); 				
			//pluginsCanvas->addWidgetDown(new ofxUIBiLabelSlider(length-xInit, 0, 100, 50, "BILABEL", "HOT", "COLD", OFX_UI_FONT_MEDIUM));

			//pluginsCanvas->addWidgetDown(new ofxUILabel("MINIMAL SLIDER", OFX_UI_FONT_MEDIUM)); 				
			//pluginsCanvas->addWidgetDown(new ofxUIMinimalSlider(length-xInit, dim, 0, 100, 50.0, "MINIMAL",OFX_UI_FONT_MEDIUM));

			//pluginsCanvas->addSpacer(length-xInit, 2);

			//pluginsCanvas->addWidgetDown(new ofxUILabel("CIRCLE SLIDER", OFX_UI_FONT_MEDIUM)); 				
			//pluginsCanvas->addWidgetDown(new ofxUICircleSlider((length-xInit)*.5, 0, 100, 50.0, "NORTH SOUTH", OFX_UI_FONT_MEDIUM));    


			commander = new CommanderTabUI();
			commander->setup();

		}


		void PluginDashboard::runtimePluginUILoader(const std::string& plugin_) {
			
			float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
			float length = lenDim -xInit; 

			std::ostringstream s;
			s<<"GUI/icons/"<<plugin_<<".png";

			pluginsCanvas->addWidgetDown(new ofxUILabel(plugin_, OFX_UI_FONT_MEDIUM)); 				
			pluginsCanvas->addWidgetDown(new ofxUIImageButton(dim*4.0, dim*4.0, true, s.str(), plugin_));
			pluginsCanvas->addSpacer(length-xInit, 2);
		}



		PluginDashboardUIManager::PluginDashboardUIManager() {
			spPluginDashboard.reset(new PluginDashboard());
		}


		void PluginDashboardUIManager::setupUI() {
			if(spPluginDashboard) {
				spPluginDashboard->setup();
				enablePluginLifecycleEvents();
			}
		}


		// TODO: we should have a queue where the events should be posted. 
		// this model seems tricky since we want to run the scripting layer in a separate thread and we want the thread to simply post the message and return
		void PluginDashboardUIManager::enablePluginLifecycleEvents() {
			ofAddListener(ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginFound, this, &PluginDashboardUIManager::onNewPlugin);
			ofAddListener(ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginLoaded, this, &PluginDashboardUIManager::onPluginLoaded);
			ofAddListener(ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginReady, this, &PluginDashboardUIManager::onPluginReady);
			ofAddListener(ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginStartError, this, &PluginDashboardUIManager::onPluginStartError);

		}

		void PluginDashboardUIManager::disablePluginLifecycleEvents() {
			ofRemoveListener(ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginFound, this, &PluginDashboardUIManager::onNewPlugin);
			ofRemoveListener(ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginLoaded, this, &PluginDashboardUIManager::onPluginLoaded);
			ofRemoveListener(ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginReady, this, &PluginDashboardUIManager::onPluginReady);
			ofRemoveListener(ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginStartError, this, &PluginDashboardUIManager::onPluginStartError);
		}

		void PluginDashboardUIManager::onNewPlugin(PluginLifecycleEventArgs& e) {

		}

		
		void PluginDashboardUIManager::onPluginLoaded(PluginLifecycleEventArgs& e) {
			// for now, we take a simple approach. render plugin on UI only if plugin has reached ready state
			//if(spPluginDashboard)
			//	spPluginDashboard->delayedCall();
		}

		void PluginDashboardUIManager::onPluginReady(PluginLifecycleEventArgs& e) {
			if(spPluginDashboard)
				spPluginDashboard->runtimePluginUILoader(e._pluginName);
		}

		void PluginDashboardUIManager::onPluginStartError(PluginLifecycleEventArgs& e) {

		}


	}
}