#pragma once
#include <boost\shared_ptr.hpp>


class ofxUIScrollableCanvas;
class PluginLifecycleEventArgs;
class CommanderTabUI;

namespace minto {
	namespace UI {

		class PluginDashboard {
			ofxUIScrollableCanvas *pluginsCanvas;
			CommanderTabUI* commander;

			float dim; 
			int lenDim;
		public:
			PluginDashboard();
			void setup();
			void runtimePluginUILoader(const std::string& plugin_);
			void overlay() {}
		};



		class PluginDashboardUIManager {
			typedef boost::shared_ptr<PluginDashboard> SPPluginDashboard;
			SPPluginDashboard spPluginDashboard;

		public:
			PluginDashboardUIManager ();
			void setupUI();
			void createPluginDashBoard();
			void overlayUserLayoutSettings();
			void enablePluginLifecycleEvents();
			void disablePluginLifecycleEvents();

			///
			/// file system monitoring found a new plugin. Should we register here? Maybe not, if we decide to move the activity console some place else.
			void onNewPlugin(PluginLifecycleEventArgs& );

			void onPluginLoaded(PluginLifecycleEventArgs& );			
			void onPluginReady(PluginLifecycleEventArgs& );
			void onPluginStartError(PluginLifecycleEventArgs& );

		};

	}

}