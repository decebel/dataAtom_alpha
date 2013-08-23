#pragma once
#include <string>

namespace gabu {

	namespace log {

		//different message types
		enum EConsoleTextType{
			CTEXT_MESSAGE = 0,
			CTEXT_COMMANDLINE,
			CTEXT_WARN,
			CTEXT_ERROR,
			CTEXT_APPMSG
		};


		struct ConsoleText {

			EConsoleTextType type;
			std::string text;
			ConsoleText() : type(CTEXT_MESSAGE), text("") {}
			ConsoleText(EConsoleTextType type_, const std::string& text_) : type(type_), text(text_) {}

		};



	}

}