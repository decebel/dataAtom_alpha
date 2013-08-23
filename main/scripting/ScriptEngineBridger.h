#pragma once

#include <boost\shared_ptr.hpp>

class ScriptEngineBridger {

public:
	void initializeEngine();
	void testEngine();
	static ScriptEngineBridger& instance();
private:

	ScriptEngineBridger();

	class ScriptEngine;
	typedef boost::shared_ptr<ScriptEngine> SPScriptEngine;
	SPScriptEngine _spScriptEngine;

};