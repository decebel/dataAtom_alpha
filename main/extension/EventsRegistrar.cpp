#include "EventsRegistrar.h"
#include <boost/unordered_map.hpp>
#include <boost/signals2/signal.hpp>

namespace minto {

	namespace ev {

		std::string AppEvent::TEXTCOMMAND = "TEXTCOMMAND"; 
		std::string AppEvent::LIVECODE = "LIVECODE"; 
		std::string AppEvent::COMMANDTEXT = "COMMANDTEXT"; 
		std::string AppEvent::PANNING	= "PANNING"; 
		std::string AppEvent::ZOOMIN	= "ZOOMIN"; 
		std::string AppEvent::ZOOMOUT	= "ZOOMOUT"; 

		struct EventsRegistrar::impl {

			impl() {
			}

			typedef boost::signals2::signal<void (void* sender, void* data )> TextCommandSignal;
			TextCommandSignal onTextCommand;

		};


		EventsRegistrar::EventsRegistrar() {
			if(!spImpl) {
				spImpl.reset(new EventsRegistrar::impl());
			}
		}



		EventsRegistrar& EventsRegistrar::instance(const std::string& context) {
			static EventsRegistrar instance;
			return instance;
		}


		// accept valid events only. Use eventName_ list
		void EventsRegistrar::registerEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith2Arg callback, bool autoEnable) {
			if(!spImpl) 
				return; // todo - setting error codes / exception 

			if(eventName_ == AppEvent::TEXTCOMMAND) 
				spImpl->onTextCommand.connect(callback);


			
		}

		void EventsRegistrar::onCommandText(uuid& id_, const std::string& sender_, const std::string& input_) {

			// pack into appropriate event args first

			// connection management stuff - disabled / prority etc 

			if(!spImpl)
				return; // todo - setting error


			spImpl->onTextCommand(NULL, NULL);



		}

	}



}