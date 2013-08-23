
#include "WindowConsoleLogger.h"
#include "ofxXmlSettings.h"
#include "ofxWindowConsole.h"

namespace gabu {

	namespace log {

		WindowConsoleLogger::WindowConsoleLogger() {

			// issue default commands for the console window renderer


			// then overlay the overrriden values
			loadSettings();

		}

		WindowConsoleLogger& WindowConsoleLogger::instance()
		{
			static WindowConsoleLogger instance;
			return instance;
		}

		void WindowConsoleLogger::setup() {
			ofxWindowConsole::instance().setup();
		}

		void WindowConsoleLogger::loadSettings(const std::string& xml) {

			ofxXmlSettings XML;
			//-----------
			//the string is printed at the top of the app
			//to give the user some feedback
			std::string message = "loading WindowConsoleLogger configuration";

			//we load our settings file
			//if it doesn't exist we can still make one
			//by hitting the 's' key
			if( XML.loadFile("WindowConsoleLogger.xml") ){
				message = "WindowConsoleLogger.xml loaded!";
			}else{
				message = "unable to load WindowConsoleLogger.xml check data/ folder";
			}

			setup();
			//enableAppEvents();

		}

		void WindowConsoleLogger::saveSettings() {

		}

		void WindowConsoleLogger::clearConsole() {
			ofxWindowConsole::instance().clearConsole();
		}

		void WindowConsoleLogger::setBackgroundColor(int red_, int green_, int blue_) {
			ofxWindowConsole::instance().setBackgroundColor(ofColor(red_, green_, blue_));
		}

		void WindowConsoleLogger::setTextColor(int red_, int green_, int blue_) {
			ofxWindowConsole::instance().setTextColor(ofColor(red_, green_, blue_));
		}

		void WindowConsoleLogger::setWarnTextColor(int red_, int green_, int blue_) {
			ofxWindowConsole::instance().setWarnTextColor(ofColor(red_, green_, blue_));
		}

		void WindowConsoleLogger::setErrorTextColor(int red_, int green_, int blue_) {
			ofxWindowConsole::instance().setErrorTextColor(ofColor(red_, green_, blue_));
		}

		void WindowConsoleLogger::printText(const ConsoleText& text_) {
			ofxWindowConsole::instance().printText(text_);
		}

		void WindowConsoleLogger::printText(const std::string& text_) {
			ofxWindowConsole::instance().printText(ConsoleText(CTEXT_MESSAGE, text_));
		}

		void WindowConsoleLogger::printWarn(const std::string& warn_) {
			ofxWindowConsole::instance().printText(ConsoleText(CTEXT_WARN, warn_));
		}

		void WindowConsoleLogger::printError(const std::string& error_) {
			ofxWindowConsole::instance().printText(ConsoleText(CTEXT_ERROR, error_));
		}

		void WindowConsoleLogger::setTextBufferLimit(int size_) {
			ofxWindowConsole::instance().setBufferSize(size_);
		}


		void WindowConsoleLogger::enableAppEvents() {
			ofxWindowConsole* console = &ofxWindowConsole::instance();
			ofAddListener(ofEvents().update, console, &ofxWindowConsole::update);
			ofAddListener(ofEvents().draw, console, &ofxWindowConsole::draw);
			ofAddListener(ofEvents().exit, console, &ofxWindowConsole::exit);
		}

		void WindowConsoleLogger::disableAppEvents() {
			ofxWindowConsole* console = &ofxWindowConsole::instance();
			ofRemoveListener(ofEvents().update, console, &ofxWindowConsole::update);
			ofRemoveListener(ofEvents().draw, console, &ofxWindowConsole::draw);
			ofRemoveListener(ofEvents().exit, console, &ofxWindowConsole::exit);
		}


	}

}