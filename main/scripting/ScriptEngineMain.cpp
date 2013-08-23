#include "ScriptEngineMain.h"
#include <boost\python.hpp>
#include <boost\python\detail\wrap_python.hpp>
#include "logging\WindowConsoleLogger.h"
#include "scriptEngineEvents.h"
#include <iostream>
#include <vector>
#include <string>
//#include "ofMain.h"



///
///\brief minto exposes the minto module for all plugin writers.
/// This module is embedded into the python startup path at startup.
/// 
/// A plugin imports this module in the standard way and call its method: 
/// <code>
/// import minto
/// print minto.api_version()
/// </code>
///

class minto {

public:
	static std::string api_version() { return "0.1"; }
	static void log_message(const std::string& sender_, const std::string& msg_) {

		using namespace gabu::log;
		WindowConsoleLogger::instance().printText(msg_);

		//std::cout<<"[minto] "<<sender_<<" : "<<msg_<<std::endl;
	}

	///
	/// \brief 
	///
	static void on_plugin_loaded(const std::string& sender_, const std::string& pluginID_) {
		using namespace gabu::log;
		WindowConsoleLogger::instance().printText("New plugin Loaded: "+pluginID_);

		ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginLoaded.notify(NULL, PluginLifecycleEventArgs(pluginID_));

	}

	///
	/// \brief 
	///
	static void on_plugin_starting(const std::string& sender_, const std::string& pluginID_) {
		using namespace gabu::log;
		WindowConsoleLogger::instance().printText("plugin starting: "+pluginID_);

		ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginStarting.notify(NULL, PluginLifecycleEventArgs(pluginID_));
	}


	///
	/// \brief 
	///
	static void on_plugin_started(const std::string& sender_, const std::string& pluginID_) {
		using namespace gabu::log;
		WindowConsoleLogger::instance().printText("plugin started: "+pluginID_);

		ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginStarted.notify(NULL, PluginLifecycleEventArgs(pluginID_));

	}

	///
	/// \brief 
	///
	static void on_plugin_ready(const std::string& sender_, const std::string& pluginID_) {
		using namespace gabu::log;
		WindowConsoleLogger::instance().printText("plugin started: "+pluginID_);

		ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginReady.notify(NULL, PluginLifecycleEventArgs(pluginID_));

	}


	///
	/// \brief 
	///
	static void on_plugin_start_error(const std::string& pluginID_, const std::string& msg_) {
		using namespace gabu::log;
		WindowConsoleLogger::instance().printText("plugin start error: "+pluginID_+" : "+msg_);

		ScriptEngineCoreEvents::getInstance().getPluginLifecycleEvents().pluginStartError.notify(NULL, PluginLifecycleEventArgs(pluginID_));
	}


};

BOOST_PYTHON_MODULE(minto)
{
	using namespace boost::python;
	def("api_version", &minto::api_version);
	def("log_message", &minto::log_message);
	def("on_plugin_loaded", &minto::on_plugin_loaded);
	def("on_plugin_starting", &minto::on_plugin_starting);
	def("on_plugin_started", &minto::on_plugin_started);
	def("on_plugin_ready", &minto::on_plugin_ready);
	def("on_plugin_start_error", &minto::on_plugin_start_error);

}

//class ScriptEngineMain::ScriptEngineThread : public ofThread {
//
//public:
//		void start(){
//          //  startThread(true, false);   // blocking, verbose
//        }
//
//        void stop(){
//           // stopThread();
//        }
//
//		void threadedFunction() {
//
//		}
//
//
//};

class ScriptEngineMain::ScriptEngine {

	PyObject* pyPluginManager;


	// error handling module objects.
	// need to clean up on exit or recycle. TODO. 
	// Not sure which ones should live as class members.
	PyObject *modStringIO;
	PyObject *obFuncStringIO;
	PyObject* _pPyobStringIO;
	PyObject* _pPyGetValFunc;
	char* pErrorString;

public:

	ScriptEngine() : modStringIO(0), obFuncStringIO(0), _pPyobStringIO(0), _pPyGetValFunc(0), pErrorString(0) {

	}

	///
	///\brief 
	///
	void initializePyEngine() {
		Py_Initialize();
	}

	void setErrorRedirection() {

		// Import cStringIO module
		modStringIO = PyImport_ImportModule("cStringIO");
		if(PyErr_Occurred() || modStringIO == NULL) {
			// log it as fatal 
			PyErr_Print();
			goto PY_INIT_ERR;
			// throw an exception. TODO
		}

		// get StringIO constructor
		obFuncStringIO = PyObject_GetAttrString(modStringIO, "StringIO");
		if(PyErr_Occurred() || obFuncStringIO == NULL){
			printf("pyParserEvaluator::Init: cant find cStringIO.StringIO:");
			PyErr_Print();
			goto PY_INIT_ERR;
		}

		// Construct cStringIO object
		_pPyobStringIO = PyObject_CallObject(obFuncStringIO, NULL);
		if(PyErr_Occurred() || _pPyobStringIO==NULL) {
			printf("pyParserEvaluator::Init: cStringIO.StringIO() failed:");
			PyErr_Print();
			goto PY_INIT_ERR;
		}

		// get getvalue() method in StringIO instance
		_pPyGetValFunc = PyObject_GetAttrString(_pPyobStringIO, "getvalue");
		if(PyErr_Occurred() || _pPyGetValFunc==NULL){
			printf("pyParserEvaluator::Init: cant find getvalue function:");
			PyErr_Print();
			goto PY_INIT_ERR;
		}

		// try assigning this object to sys.stderr
		int ret = PySys_SetObject("stderr", _pPyobStringIO);
		if(ret != 0) {
			printf("failed to assign _pPyobStringIO to stderr\n");
			goto PY_INIT_ERR;
		}

		return;

PY_INIT_ERR:
		Py_XDECREF(modStringIO);
		Py_XDECREF(obFuncStringIO);
		Py_XDECREF(_pPyobStringIO);
		Py_XDECREF(_pPyGetValFunc);
		// TODO: This error needs to be reported to the UI 

	}

	void pre_engine_setup() {
		PyImport_AppendInittab("minto", initminto);
	}


	int getPyErrorMessage() {

		PyErr_Print();

		// call getvalue() method in StringIO instance
		int ret = 0;
		PyObject *obResult=NULL;
		char *sresult = NULL;
		obResult = PyObject_CallObject(_pPyGetValFunc, NULL);
		if(PyErr_Occurred() || obResult==NULL){
			printf("getvalue() failed\n");
			ret = -1;
			goto CLEAN_AND_RETURN;
		}
		// did getvalue return a string?
		if(!PyString_Check(obResult)){
			printf("getvalue() did not return error string\n");
			ret = -1;
			goto CLEAN_AND_RETURN;
		}
		// retrieve error message string from this object
		if(NULL != (sresult = PyString_AsString(obResult))){
			pErrorString = strdup(sresult);
			minto::log_message("", std::string(pErrorString));
		} else {
			// we need to cry like a baby now
			ret = -1;
			goto CLEAN_AND_RETURN;
		}
		return(ret);

CLEAN_AND_RETURN:
		Py_XDECREF(obResult);
		return(ret);
	}

	void set_plugin_HomeBase() {

		PyObject *sys_path; 
		PyObject *path; 

		sys_path = PySys_GetObject("path"); 
		if (sys_path == NULL) {
			printf("\ncannot get path from sys module");
			return; 
		}

		path = PyString_FromString("C:\\Amit\\dev\\cpp\\of_v0.7.4_vs2010_release\\of_v0.7.4_vs2010_release\\apps\\myApps\\alpha-goldDust\\bin\\plug\\py\\api") ;
		if (path == NULL) {
			printf("Error setting path");
			goto PY_PLUGPATH_CLEAN;
		}
		if (PyList_Append(sys_path, path) < 0) 
			goto PY_PLUGPATH_CLEAN;

		//return;
PY_PLUGPATH_CLEAN:
		Py_XDECREF(path); // we want to clean even if there is no error since list append automatically increments the reference.
		// we must Py_XDECREF(path). Otherwise we have a mem leak.
		return;
	}

	///
	///\brief imports the main py module we are using for this framework
	///
	void importPluginsController() {

		pyPluginManager = PyImport_ImportModule("drawingboard");

		if(PyErr_Occurred() || pyPluginManager == NULL) {
			getPyErrorMessage();
			goto CLEANUP_MAINMOD;			
		}

		PyObject* pyVer = NULL;
		pyVer = PyObject_GetAttrString(pyPluginManager, "version");
		if(PyErr_Occurred() || pyVer == NULL) {
			getPyErrorMessage();
			goto CLEANUP_MAINMOD;
		}

		/* convert version info into c string */
		char* verStr = NULL;
		int ret = PyArg_Parse(pyVer, "s", &verStr);
		if(!ret) {
			getPyErrorMessage();
			goto CLEANUP_MAINMOD;
		}
		std::cout<<"ctrl module version: "<<verStr<<std::endl;
		Py_DECREF(pyVer);

		return ;
		// version compatibilty check - TODO


CLEANUP_MAINMOD:

		minto::log_message("", std::string(pErrorString));
		std::cout<<"error: "<<pErrorString<<std::endl;
		Py_XDECREF(pyPluginManager);
		Py_XDECREF(pyVer);		

	}

	void load_plugins() {

		if(pyPluginManager == NULL) {
			minto::log_message("", "plugin manager is null");
			// we need to set some condition or throw an exception for GUI indication
			return;
		}

		PyObject* pyload_plugin_method = 0;
		pyload_plugin_method = PyObject_GetAttrString(pyPluginManager, "test_main"); //"load_plugins");
		if(PyErr_Occurred() || pyload_plugin_method == NULL) {
			getPyErrorMessage();
			goto CLEANUP_ERR_PLUGS;
		}

		//todo - measure performance 

		PyObject* pycall_result = 0;
		pycall_result = PyEval_CallObject(pyload_plugin_method, NULL);
		if(PyErr_Occurred() || pycall_result == NULL) {
			getPyErrorMessage();
			goto CLEANUP_ERR_PLUGS;
		}

		PyObject* pyplugin_iterator = 0;
		pyplugin_iterator = PyObject_GetIter(pycall_result);
		if(PyErr_Occurred() || pyplugin_iterator == NULL) {
			getPyErrorMessage();
			goto CLEANUP_ERR_PLUGS;
		}

		PyObject* pyplugin_instance;
		while(pyplugin_instance = PyIter_Next(pyplugin_iterator)) {
			std::cout<<"found plugin instance"<<std::endl;
			//add to PluginHolder::addPlugin()
		}

		// after all the plugins have been instantiated, we can call a test method an verify
		PyObject* pyload_post_plugin_method = 0;
		pyload_post_plugin_method = PyObject_GetAttrString(pyPluginManager, "testExecute"); 
		if(PyErr_Occurred() || pyload_post_plugin_method  == NULL) {
			getPyErrorMessage();
			goto CLEANUP_ERR_PLUGS;
		}

		//todo - measure performance 
		PyObject* pycall_testExecute_result = 0;
		pycall_testExecute_result = PyEval_CallObject(pyload_post_plugin_method, NULL);
		if(PyErr_Occurred() || pycall_testExecute_result == NULL) {
			getPyErrorMessage();
			goto CLEANUP_ERR_PLUGS;
		}

		goto CLEANUP_PLUG_REFS;

CLEANUP_ERR_PLUGS:
		std::cout<<"ERROR: "<<std::string(pErrorString)<<std::endl;
		minto::log_message("", std::string(pErrorString));

CLEANUP_PLUG_REFS:
		// TODO : check internet connection. else we will have crashes 
		if(pycall_testExecute_result)
			Py_XDECREF(pycall_testExecute_result); // if the result failed, then freeing causes crash !! Amits
		if(pyload_post_plugin_method)
			Py_XDECREF(pyload_post_plugin_method);
		if(pyplugin_iterator)
			Py_XDECREF(pyplugin_iterator);
		if(pycall_result)
			Py_XDECREF(pycall_result);
		if(pyload_plugin_method)
			Py_XDECREF(pyload_plugin_method);		

	}


	void testEngine() {

	}

	void execute(const std::string& name, const std::string& command) {

		// after all the plugins have been instantiated, we can call a test method an verify
		PyObject* pyplugin_command_method = 0;
		pyplugin_command_method = PyObject_GetAttrString(pyPluginManager, "execute_data_command"); 
		if(PyErr_Occurred() || pyplugin_command_method  == NULL) {
			getPyErrorMessage();
			goto CLEANUP_ERR;
		}

		PyObject* arglist = NULL;
		PyObject* result = NULL;

		arglist = Py_BuildValue("ss", name.c_str(), command.c_str());


		//todo - measure performance 
		PyObject* pycall_execute_result = 0;
		pycall_execute_result = PyEval_CallObject(pyplugin_command_method, arglist);
		if(PyErr_Occurred() || pycall_execute_result == NULL) {
			getPyErrorMessage();
			goto CLEANUP_ERR;
		}


CLEANUP_ERR:
		// TODO : check internet connection. else we will have crashes 
		if(pyplugin_command_method)
			Py_XDECREF(pyplugin_command_method); // if the result failed, then freeing causes crash !! Amits
		if(arglist)
			Py_XDECREF(arglist); 


	}


};

ScriptEngineMain& ScriptEngineMain::instance() {
	static ScriptEngineMain seMain;
	return seMain;
}

ScriptEngineMain::ScriptEngineMain() {
	_spScriptEngine.reset(new ScriptEngineMain::ScriptEngine());
}

void ScriptEngineMain::initializeEngine() {

	if(!_spScriptEngine) {
		// todo - throw exception
		std::cout<<"ScriptEngine not initialized"<<std::endl;
		return;
	}
	_spScriptEngine->pre_engine_setup();
	_spScriptEngine->initializePyEngine();
	_spScriptEngine->setErrorRedirection();
	_spScriptEngine->set_plugin_HomeBase();
	_spScriptEngine->importPluginsController();
	_spScriptEngine->load_plugins();
	_spScriptEngine->execute("facebook", "find cme");


	// todo 
	//extract callbacks handlers 

}

void ScriptEngineMain::testEngine() {
	if(!_spScriptEngine) {
		//throw error
		return;
	}

	_spScriptEngine->testEngine();
}

void ScriptEngineMain::enque(UserCommand& cmd) {

	// for now, I think its easier to delegate this job to the python layer by calling a py function
	// py function can decide what plugin it has loaded and call it appropriately.

	if(cmd.handlerKey == "FB" || cmd.handlerKey == "fb") {

	} else if(cmd.handlerKey == "TW" || cmd.handlerKey == "tw") {

	} else {

	}




}