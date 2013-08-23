#ifndef EVENTSREGISTRAR_H
#define EVENTSREGISTRAR_H

#include <string>
#include <boost\shared_ptr.hpp>

namespace minto {

	namespace ev {



		typedef void (*callbackWith0Arg)();
		typedef void (*callbackWith1Arg)(void* sender);
		typedef void (*callbackWith2Arg)(void* sender, void* data);
		typedef void (*callbackWith3Arg)(void* sender, void* data1, void* data2);
		typedef void (*callbackWith4Arg)(void* sender, void* data1, void* data2, void* data3);
		typedef void (*callbackWith5Arg)(void* sender, void* data1, void* data2, void* data3, void* data4);


		struct uuid {
			int id;
			uuid() : id(-1) {}
			uuid(int id_) : id(id_) {}
		};

		
		struct AppEvent {
			static std::string TEXTCOMMAND;
			static std::string LIVECODE;
			static std::string COMMANDTEXT;
			static std::string PANNING;
			static std::string ZOOMIN;
			static std::string ZOOMOUT;
		};
		
		class EventsRegistrar {

		private:
			EventsRegistrar ();
			EventsRegistrar(const EventsRegistrar& );
			EventsRegistrar& operator = (const EventsRegistrar& ) ;

		public:

			static EventsRegistrar& instance(const std::string& context="default");

			/// 
			/// events
			///
			void onTextInput(uuid& id_, const std::string& sender_, const std::string& input_);
			void onCommandText(uuid& id_, const std::string& sender_, const std::string& input_);
			void onLiveCode (uuid& id_, const std::string& sender_, const std::string& code_);
			

			
			/// 
			/// registration / toggle settings for events
			void registerEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith0Arg callback, bool autoEnable = true);
			void registerEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith1Arg callback, bool autoEnable = true);
			void registerEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith2Arg callback, bool autoEnable = true);
			void registerEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith3Arg callback, bool autoEnable = true);
			void registerEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith4Arg callback, bool autoEnable = true);

			void disableEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith0Arg callback);
			void disableEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith1Arg callback);
			void disableEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith2Arg callback);
			void disableEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith3Arg callback);
			void disableEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith4Arg callback);

			void enableEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith0Arg callback);
			void enableEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith1Arg callback);
			void enableEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith2Arg callback);
			void enableEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith3Arg callback);
			void enableEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith4Arg callback);

			void removeEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith0Arg callback);
			void removeEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith1Arg callback);
			void removeEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith2Arg callback);
			void removeEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith3Arg callback);
			void removeEventListener(const std::string& eventName_, const std::string& handlerName_, callbackWith4Arg callback);


		private:
			struct impl;
			boost::shared_ptr<impl> spImpl;			



		};

	}

}

#endif