#ifndef SCRIPTENGINEEVENTARGS_H
#define SCRIPTENGINEEVENTARGS_H

#include "ofEvents.h"
#include <string>

// todo - json style for multi args (or a variant map if it suffice )
struct CommandDetails {
	std::string commandText;
	CommandDetails() : commandText("") {}
	CommandDetails(const std::string& cmd) : commandText(cmd) {}

};

struct UserCommand {
	int id;
	std::string handlerKey; //who should handle the execution of the command. 
	CommandDetails subCommand; // command details;

	UserCommand() : id(-1), handlerKey("") {}
	UserCommand(int Id, const std::string& key) : id(Id), handlerKey (key) {}
};

///
/// \brief events related to plugin lifecycle such as:
/// plugin found, plugin loaded, plugin started, plugin start error, plugin stopped, plugin suspended
///

class PluginLifecycleEventArgs : public ofEventArgs {

public:
	std::string _pluginName;

	PluginLifecycleEventArgs(const std::string& pluginName_) : _pluginName(pluginName_) {}

};

class PluginLifecycleEvents {

public:
	ofEvent<PluginLifecycleEventArgs> pluginFound;
	ofEvent<PluginLifecycleEventArgs> pluginLoading;
	ofEvent<PluginLifecycleEventArgs> pluginLoaded;
	ofEvent<PluginLifecycleEventArgs> pluginStarting;
	ofEvent<PluginLifecycleEventArgs> pluginStarted; // started and ready are two separate things. 
	ofEvent<PluginLifecycleEventArgs> pluginReady; // ready means the plugin's is_initialized call returns True
	ofEvent<PluginLifecycleEventArgs> pluginStartError;
	ofEvent<PluginLifecycleEventArgs> pluginStopping;
	ofEvent<PluginLifecycleEventArgs> pluginStopped;
	ofEvent<PluginLifecycleEventArgs> pluginStopError;
	ofEvent<PluginLifecycleEventArgs> pluginSuspending;
	ofEvent<PluginLifecycleEventArgs> pluginSuspended;
	ofEvent<PluginLifecycleEventArgs> pluginResuming;
	ofEvent<PluginLifecycleEventArgs> pluginResumed;

	void disable() {

	}

	void enable() {

	}
};





class ScriptEngineCoreEvents {
	
	ScriptEngineCoreEvents();
	ScriptEngineCoreEvents(const ScriptEngineCoreEvents& );
	ScriptEngineCoreEvents& operator = (const ScriptEngineCoreEvents&) ;

	PluginLifecycleEvents* _plugLifecycleEvents;

public:
	static ScriptEngineCoreEvents& getInstance();
	PluginLifecycleEvents& getPluginLifecycleEvents();
	~ScriptEngineCoreEvents();

};



#endif