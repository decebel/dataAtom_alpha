#include "ScriptEngineBridger.h"
#include <vector>
#include <boost/python.hpp>

namespace python = boost::python;

// An abstract base class
class Base : public boost::noncopyable
{
public:
	virtual ~Base() {};
	virtual std::string hello() = 0;
};

// C++ derived class
class CppDerived : public Base
{
public:
	virtual ~CppDerived() {}
	virtual std::string hello() { return "Hello from C++!";}
};



// Familiar Boost.Python wrapper class for Base
struct BaseWrap : Base, python::wrapper<Base>
{
	virtual std::string hello() 
	{
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
		// workaround for VC++ 6.x or 7.0, see
		// http://boost.org/libs/python/doc/tutorial/doc/html/python/exposing.html#python.class_virtual_functions
		return python::call<std::string>(this->get_override("hello").ptr());
#else
		std::string s("hello");
		return python::call<std::string>(this->get_override("hello").ptr());
		//return this->get_override("hello")();
#endif
	}
};

/// 
/// \TODO: move to appropriate header
struct DataSchema {

};


class DataDriver {

};

class DataDriverStats {

};


///
/// \class SpringBoardDataManager 
/// \brief main plugin manager class to be exposed for script writers
///


struct SpringBoardDataManager {


	static std::string version() {
		return "0.1";
	}

	///
	/// \brief plugins can use this method to perform app logging 
	///
	/// \param[in] msg_ message to log
	/// \return void
	///

	static void log_message(const std::string& msg_) {

	}

	///
	/// \brief plugins can use this method to display an error message
	///
	/// \param[in] msg_ message to display
	/// \param[in] sender_ sender name. usually the plugin name
	///

	static void error_message(const std::string& msg_, const std::string& sender_) {

	}

	static std::vector<std::string> get_all_market_names() {

		std::vector<std::string> v;
		return v;
	}


	static std::vector<std::string> get_market_streams(const std::string& marketname_) {
		std::vector<std::string> v;
		return v;
	}


	static DataSchema get_stream_schema(const std::string& stream_) {
		return DataSchema();
	}

	static DataDriver* get_input_datadriver(const std::string& driver_) {
		return 0;
	}

	static DataDriver* get_output_datadriver(const std::string& driver_) {
		return 0;
	}

	static DataDriverStats  get_stream_stats(const std::string& stream_) {
		return DataDriverStats ();
	}


	static bool set_new_stream(const std::string& streamname_, const DataSchema& schema_, const std::string& market_) {
		return true;
	}

	static void bind(const std::string& stream_, DataDriver* driver_, const std::string& filter_) {

	}

	static void unbind(const std::string& stream_, const DataDriver* driver_, const std::string& filter_) {

	}

	static void onData(const std::string& stream_, const std::string& tuple) {

	}
};

// Pack the Base class wrapper into a module
BOOST_PYTHON_MODULE(embedded_hello)
{
	python::class_<BaseWrap, boost::noncopyable> base("Base");
}

BOOST_PYTHON_MODULE(sbdatamanager)
{
	python::class_<SpringBoardDataManager, boost::noncopyable>("SpringBoardDataManager")
		.def("version", &SpringBoardDataManager::version)
		.staticmethod("version")
		.def("log_message", &SpringBoardDataManager::log_message)
		.staticmethod("log_message")
		.def("error_message", &SpringBoardDataManager::error_message)
		.staticmethod("error_message")
		.def("get_all_market_names", &SpringBoardDataManager::get_all_market_names)
		.staticmethod("get_all_market_names")
		.def("get_market_streams", &SpringBoardDataManager::get_market_streams)
		.staticmethod("get_market_streams")
		.def("get_stream_schema", &SpringBoardDataManager::get_stream_schema)
		.staticmethod("get_stream_schema")
		//.def("get_input_datadriver", &SpringBoardDataManager::get_input_datadriver)
		//.def("get_output_datadriver", &SpringBoardDataManager::get_output_datadriver)
		//.def("get_stream_stats", &SpringBoardDataManager::get_stream_stats)
		//.def("set_new_stream", &SpringBoardDataManager::set_new_stream)
		//.def("bind", &SpringBoardDataManager::bind)
		//.def("unbind", &SpringBoardDataManager::unbind)
		//.def("onData", &SpringBoardDataManager::onData)

		;

}


void exec_test()
{
	std::cout << "registering extension module embedded_hello..." << std::endl;

	// Initialize the interpreter
	Py_Initialize();

	// Register the module with the interpreter
	if (PyImport_AppendInittab("embedded_hello", initembedded_hello) == -1)
		throw std::runtime_error("Failed to add embedded_hello to the interpreter's "
		"builtin modules");

	std::cout << "defining Python class derived from Base..." << std::endl;

	// Retrieve the main module
	python::object main = python::import("__main__");

	// Retrieve the main module's namespace
	python::object global(main.attr("__dict__"));

	// Define the derived class in Python.
	python::object result = python::exec(
		"from embedded_hello import *        \n"
		"class PythonDerived(Base):          \n"
		"    def hello(self):                \n"
		"        return 'Hello from Python!' \n",
		global, global);


	// Define the plugin manager class in Python.
	python::object result_sbdm = python::exec(
		"from sbdatamanager import SpringBoardDataManager   \n",
		global, global);

	python::object PythonDerived = global["PythonDerived"];

	// Creating and using instances of the C++ class is as easy as always.
	CppDerived cpp;
	//BOOST_TEST(cpp.hello() == "Hello from C++!");

	std::cout << "testing derived class from C++..." << std::endl;

	// But now creating and using instances of the Python class is almost
	// as easy!
	python::object py_base = PythonDerived();
	Base& py = python::extract<Base&>(py_base) BOOST_EXTRACT_WORKAROUND;

	// Make sure the right 'hello' method is called.
	//BOOST_TEST(py.hello() == "Hello from Python!");

	std::string script_ = "C:\\Amit\\dev\\cpp\\of_v0.7.4_vs2010_release\\of_v0.7.4_vs2010_release\\apps\\myApps\\alpha-goldDust\\bin\\data\\packages\\springboard.py";
	python::object fileresult = python::exec_file(script_.c_str(), global, global);

	python::object input_dd = global["create_input_datadrivers2"];
	python::object input_dd_objects = input_dd();
	python::object iter_obj = python::object( python::handle<>( PyObject_GetIter( input_dd_objects.ptr() ) ));

	python::object obj = python::extract<python::object>( iter_obj.attr( "next" )() );
	int val = python::extract<int>( obj );



	//int len = python::extract<int>(input_dd_objects.attr("count"));
	std::cout<<"len = "<<val<<std::endl;

	std::cout << "success!" << std::endl;
}

ScriptEngineBridger& ScriptEngineBridger::instance() {
	static ScriptEngineBridger instance;
	return instance;
}

class Scope {

public:

	python::object main;
	python::object main_dict;
	python::object datamanager;
	python::object result_sbdm;

	void initialize() {

		// Initialize the interpreter
		Py_Initialize();

		// Retrieve the module
		main = python::import("__main__");

		// Retrieve the module's namespace
		main_dict  = (main.attr("__dict__"));

		// Register the module with the interpreter
		if (PyImport_AppendInittab("sbdatamanager", initsbdatamanager) == -1)
			throw std::runtime_error("Failed to add sbdatamanager to the interpreter's "
			"builtin modules");

		std::cout << "defining Python class sbdatamanager ..." << std::endl;

		// Define the plugin manager class in Python.
		result_sbdm = python::exec(
			"from sbdatamanager import SpringBoardDataManager   \n",
			main_dict, main_dict);

		// Define the plugin manager class in Python.
		python::object versionResult = python::exec(
			"sb_ver = SpringBoardDataManager.version()   \n",
			main_dict, main_dict);

		std::string ver = python::extract<std::string>(main_dict["sb_ver"]);
		std::cout<<"springBoardDataManager version = "<<ver<<std::endl;

		// version check code and make sure the class is loaded correctly
		//python::object springBoardDataManager = main_dict["SpringBoardDataManager"];

		//std::string versionStr = python::extract<std::string>(springBoardDataManager.attr("version"));

		//std::cout << "SpringBoardDataManager version ..." <<versionStr<< std::endl;
	}

	void loadPluginManagerModule() {

	}

	void loadPlugins(const std::string& scriptFile_) {

		std::string script_ = "C:\\Amit\\dev\\cpp\\of_v0.7.4_vs2010_release\\of_v0.7.4_vs2010_release\\apps\\myApps\\alpha-goldDust\\bin\\data\\packages\\springboard.py";
		python::object result = python::exec_file(script_.c_str(), main_dict, main_dict);

		// now we need to call the plugin boostrapper
		python::object plugin_ld = main_dict["plugin_bootstrapper"];
		plugin_ld();

		// now we need to get the loaded plugins
		python::object in_plugin_ld = main_dict["create_output_datadrivers"];
		python::object in_plugins = in_plugin_ld();

		python::list l = python::extract<python::list>(in_plugins);
		long res = l.count(10);

		python::object obj_plugin  = l[0];
		python::object obj_plugin_method = python::extract<python::object>( obj_plugin.attr( "onData" )() );
		obj_plugin_method("test", "testdata");

		//python::object iter_in_plugin = python::object( python::handle<>( PyObject_GetIter( in_plugins.ptr() ) ));
		//python::object obj_plugin = python::extract<python::object>( iter_in_plugin.attr( "next" )() );

		//python::object obj_plugin_method = python::extract<python::object>( obj_plugin.attr( "onData" )() );
		//obj_plugin_method("test", "testdata");
		//int val = python::extract<int>( obj );

		

	}




};

class ScriptEngineBridger::ScriptEngine {

	Scope _scope;

public:
	
	ScriptEngine() {}

	void initialize() {

	}

	void testEngine() {
		//exec_test();
		_scope.initialize();
		//_scope.loadPlugins("");
	}
};

ScriptEngineBridger::ScriptEngineBridger() {
	initializeEngine();
}

void ScriptEngineBridger::initializeEngine() {

	_spScriptEngine.reset(new ScriptEngineBridger::ScriptEngine());
}

void ScriptEngineBridger::testEngine() {
	if(_spScriptEngine) 
		_spScriptEngine->testEngine();
}

