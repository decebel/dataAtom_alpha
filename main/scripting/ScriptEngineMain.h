#pragma once

#include <boost\shared_ptr.hpp>
#include <string>

class ScriptableAppEvents {

public:
	// todo - need to identify various callback handler methods in the main plugin manager class and cache them as required
	static void performBindingToScriptingLayer(); 
	static void onCommandText(const std::string& sender, const std::string& text);
	

};

struct UserCommand;

///
///\class ScriptEngineMain 
///\brief This class is responsible for all connection with the rest of the framework. Using signals/callbacks scheme.
///

class ScriptEngineMain {

public:
	void initializeEngine();
	void testEngine();
	static ScriptEngineMain& instance();

	// user command 
	void enque(UserCommand& cmd);

private:

	ScriptEngineMain();

	//class ScriptEngineThread;
	//typedef boost::shared_ptr<ScriptEngineThread> SPScriptEngineThread;
	//SPScriptEngineThread _spScriptEngineThread;


	class ScriptEngine;
	typedef boost::shared_ptr<ScriptEngine> SPScriptEngine;
	SPScriptEngine _spScriptEngine;

};


