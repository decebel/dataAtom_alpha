"""API for plugin writers to adhere to.

Engine shall instantiate the drawingboard module.
"""

version = "1.0"

import os
plugSourcesPath = "C:\\Amit\\dev\\cpp\\of_v0.7.4_vs2010_release\\of_v0.7.4_vs2010_release\\apps\\myApps\\alpha-goldDust\\bin\\plug\\py\\sources" ##os.path.join("..", "sources")
###############################################################
### TODO. we should not explicitly set the paths here. They should be automatically 
#### by done by the __init__ in sources 
webplugSourcesPath = os.path.abspath(os.path.join(plugSourcesPath, "weby"))
#print("web plug path: {0}").format(os.path.abspath(webplugSourcesPath))
import sys; sys.path.insert(0, webplugSourcesPath)

import pprint
import imp
## TODO: we should provide a proxy minto module for non-embedded testing
try:
	import minto
	print("minto api: {0}").format(minto.api_version())
	minto.log_message("", version+" loaded")
except:
	print("You must be in testing mode since minto is not available")




pp = pprint.PrettyPrinter(indent=4)
module_plugins = []
plugin_objects = []

# key-value pair for pluginName to pluginObject
plugin_commands = {}

class DrawingBoardInitError(Exception):
	pass


class drawingboard(object):
	"""static class drawingboard instantiated by the Engine"""
	def __init__(self):
		raise DrawingBoardInitError("static class")

	def version(self):
		pass

	def log_message(self, sender, data):
		pass

	def log_error(self, sender, data):
		pass

	def get_all_market_names(self):		
		pass

	def get_all_market_streams(self, market = "default"):
		pass

	def get_schema_for_stream(self, stream):
		pass

	def get_input_command(self, commandname):
		pass

	def get_output_command(self, commandname):
		pass

	def get_all_input_command(self):
		pass

	def get_all_output_command(self):
		pass

	def get_stats_for_command(self, commandname):
		pass

	def set_new_stream(self, stream, schema, provider):
		pass

	def set_provider(self, stream, provider):
		pass

	def register_provider(self, stream, filter, provider):
		pass

	def unregister_provider(self, stream, provider):
		pass

	def onData(self, stream, driver, data):
		pass

	# events - TODO
	# Document the methods, remove redundant, provide example usages

# TODO - doc strings and group the apis based on usage. caller etc
class DataCommand(object):

	is_command = True

	def __init__(self, **kwargs):
		print "DataCommand init called"
		self.args = {}		

	def set_defaults(self):
		self.set_display_name("TODO") #we should try getting a name from the subclass name. TODO.
		self.set_status_string("initializing")
		# TODO: intelligent about the png. Use class name to figure out the icon. If not found, 
		# then use the default png
		self.set_icon("plugin.png")
		self.set_command_name("noname")
		self.set_display_name("noname")
		self.set_initialized(False)

	def set_command_name(self, name):
		self.args['command_name'] = name

	def get_command_name(self):
		return self.args['command_name']

	def set_display_name(self, name):
		self.args['display_name'] = name		

	def get_display_name(self):
		return self.args['display_name']

	def get_status_string(self):
		return self.args['status_string']

	def set_status_string(self, status):
		self.args['status_string'] = status

	def set_icon(self, resource):
		self.args['icon'] = resource

	def get_icon(self):
		return self.args['icon']

	def init(self, **kwargs):
		pass

	# user defined plugin has to explicitly initialize a plugin when they know they are ready
	def is_initialized(self):
		return self.args['initialized'] 

	def set_initialized(self, status):
		self.args['initialized'] = status

	def load_resources(self, **kwargs):
		self.set_defaults()

	def _start(self):
		pass

	def _stop(self):
		pass


	def start(self, **kwargs):
		pass


	def stop(self):
		pass

	def is_stopped(self):
		pass

	def suspend(self):
		pass

	def is_suspended(self):
		pass

	def shutdown(self):
		pass

	def is_shutdown(self):
		pass

	def can_batch(self):
		pass

	def set_batchsize(self, size):
		pass

	def get_batchsize(self):
		pass

	def set_flush_interval(self, interval):
		pass

	def get_flush_interval(self):
		pass

	def subscribe_my_stats(self, callback):
		"""If someone is interested in the DataCommand stats, they can provide a callback."""
		pass

	def on_new_stream(self, stream, schema):
		pass

	def set_port_hints(self):
		pass

	# should we make the get/set as properties?	
	def set_input_port_count(self, count):
		pass

	def set_output_port_count(self, count):
		pass

	def get_input_port_count(self):
		pass

	def get_output_port_count(self):
		pass

	def set_output_port_schema(self, port, schema):
		pass

	def get_output_port_schema(self, port):
		pass

	def set_input_port_schema(self, port, schema):
		pass

	def get_input_port_schema(self, port):
		pass

	def submit_command(self, port, **commandArgs):
		pass

	def execute(self, **kwargs):
		pass

	def send_output(self, port, data):
		pass

	def send_output_async(self, port, data):
		pass

	def cancel_last_command(self):
		pass

	def is_command_canceled(self):
		pass

	def send_error_port(self, data):
		pass

	def send_admin_port(self, status):
		pass

	def has_admin_port(self):
		pass

	def check_connections(self):
		"""typecheck schema for ports."""
		pass

	def get_params(self):
		pass

	def set_params(self, **kwParams):
		pass

	#event subscription todo

	def resume(self):
		pass

	def is_resumed(self):
		pass

def list_directory(directory, fileExt = [".py"]):
	fileList = [f for f in os.listdir(directory)]
	fileList = [os.path.splitext(f)[0] for f in fileList if os.path.splitext(f)[1] in fileExt]
	print "fileList {0}".format(fileList)
	return fileList


def load_data_plugins(directory, fileExt = [".py"]):
	modNames = list_directory(directory, fileExt)

	for moduleName in modNames:
		print "Checking module {0}".format(moduleName)
		if moduleName == "drawingboard":
			print "Skipping module {0}".format(moduleName)
			continue
		m_info = imp.find_module(moduleName)
		m = imp.load_module(moduleName, *m_info)

		for name in dir(m):
			t = m.__dict__[name]
			try:
				if t.__bases__:
					try:
						if t.is_command and name != "DataCommand":
							minto.log_message("", "Found plugin class: "+name)
							print "Found plugin class: {0}".format(name)
							module_plugins.append(t)
					except AttributeError:
						pass
						#print "class: {0} not a plugin class".format(name)
			except AttributeError:
				pass

def get_command_name(commandClass):
	return str(commandClass)

def load_plugin_commands():
	for klass in module_plugins:
		load_plugin_command(klass)

def load_plugin_command(klass):
	pluginObject = klass(dummy="dummy")
	pluginObject.load_resources(dummy="dummy")


	# TODO
	# we should first try to get the name from the plugin object and then use the generic routine

	name = pluginObject.get_display_name() #get_command_name(klass)
	print "command name: {0}".format(name)
	plugin_commands[name] = pluginObject
	minto.on_plugin_loaded("db", name)

def create_data_commands():
	cmds = []
	for (plugName, plugObject) in plugin_commands.iteritems():
		create_data_command(plugName, plugObject)
		cmds.append(plugObject)

	return cmds

def create_data_command(pluginName, plugin):
	if plugin.is_initialized():
		minto.on_plugin_starting("", pluginName)
		plugin.start(en="english")
		minto.on_plugin_ready("", pluginName)
	else:
		minto.on_plugin_start_error(pluginName, "plugin not initialized")	

def execute_data_command(pluginName, pluginCommand):
        #global plugin_commands
        if pluginName in plugin_commands:
                print "executing for {0}: {1}".format(pluginName, pluginCommand)
                minto.log_message("", "executing: {0} with {1}".format(pluginName, pluginCommand))
                #plugin_commands[pluginName].execute(pluginCommand)
        else:
                minto.log_message("", "{0}: not found".format(pluginName))
        

# def create_data_commands():
# 	cmds = []
# 	for cls in module_plugins:
# 		pluginObject = cls(dummy="dummy")
# 		pluginObject.start(en="english")
# 		cmds.append(pluginObject)
# 		plugin_objects.append(pluginObject)
# 	return cmds

def testExecute():
	for pluginObject in plugin_objects:
		if pluginObject.is_initialized():
			pluginObject.execute(search = "now is the time")

def test_main():	
	pp.pprint(sys.path)

	#webPlugPathTest = os.path.join(plugSourcesPath, "weby")
	print("webplugSourcesPath: {0}").format(webplugSourcesPath) #webPlugPathTest)
	minto.log_message("", "webplugSourcesPath: "+webplugSourcesPath)
	load_data_plugins(webplugSourcesPath) #webPlugPathTest)
	print("completed identifying plugins")
	minto.log_message("", "completed identifying plugins")
	load_plugin_commands()
	cmds = create_data_commands()
	return cmds

def main():
	pp.pprint(sys.path)

	webPlugPathTest = os.path.join(plugSourcesPath, "weby")
	load_data_plugins(os.path.abspath(webPlugPathTest))
	cmds = create_data_commands()
	plugin_objects.extend(cmds)

if __name__ == '__main__':
	main()
	testExecute()















