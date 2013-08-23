#include "scriptEngineEvents.h"

ScriptEngineCoreEvents::ScriptEngineCoreEvents() : _plugLifecycleEvents(0) {

	_plugLifecycleEvents = new PluginLifecycleEvents();
}

ScriptEngineCoreEvents& ScriptEngineCoreEvents::getInstance() {
	static ScriptEngineCoreEvents instance;
	return instance;
}

PluginLifecycleEvents& ScriptEngineCoreEvents::getPluginLifecycleEvents() {
	return *_plugLifecycleEvents;
}

ScriptEngineCoreEvents::~ScriptEngineCoreEvents() {
	if(_plugLifecycleEvents) {
		delete _plugLifecycleEvents;
		_plugLifecycleEvents = 0;
	}
}

