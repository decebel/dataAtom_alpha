#pragma once
#include <string>
#include "ConsoleLoggingUtil.h"

namespace gabu {

	namespace log {

		class WindowConsoleLogger {

			WindowConsoleLogger ();
			WindowConsoleLogger(const WindowConsoleLogger&);
			WindowConsoleLogger& operator = (const WindowConsoleLogger&);

			
			

		public:

			static WindowConsoleLogger& instance();
			
			void setup();

			// we want to keep this private?
			void loadSettings(const std::string& xml = "WindowConsoleLogger.xml");			

			// TODO - methods for saving configuration - such as color change etc
			void saveSettings();

			void clearConsole();
			void setBackgroundColor(int red, int green, int blue);
			void setTextColor(int red, int green, int blue);
			void setWarnTextColor(int red, int green, int blue);
			void setErrorTextColor(int red, int green, int blue);
			void printText(const ConsoleText& text);
			void printText(const std::string& text);
			void printWarn(const std::string& warn);
			void printError(const std::string& error);
			void setTextBufferLimit(int size); // # of lines after which we clear old log lines. Command control. phew!!
			
			void enableAppEvents();
			void disableAppEvents();


		};

	}

}