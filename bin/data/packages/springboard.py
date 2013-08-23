

import os
import sys
import time
#import springboarddatamanager
import imp

application_op_classes = []
window_op_classes = []
inputdata_op_classes = []
outputdata_op_classes = []
view_op_classes = []

all_op_classes = [application_op_classes, window_op_classes, inputdata_op_classes, outputdata_op_classes, view_op_classes]

all_callbacks = {'on_new': [], 'on_clone': [], 'on_load': [], 'on_close': [],
    'on_pre_save': [], 'on_post_save': [], 'on_modified': [],
    'on_selection_modified': [],'on_activated': [], 'on_deactivated': [],
    'on_project_load': [], 'on_project_close': [], 'on_query_context': [],
    'on_query_completions': []}


class DataDriver(object):

        def setShortDisplayName(self, name):
            """Returns a tooltip Name."""
            pass

        def getShortDisplayName(self):
            pass

        def getStatusString(self):
            pass

        def setStatusString(self):
            pass

        def setIcon(self, icon):
            pass

        def getIcon(self):
            pass

        def resume(self):
            """Called by the App for the driver to resume."""
            pass

        def resumed(self):
            """returns true when the driver is resumed."""
            pass

        def suspend(self):
            """Called by the App to suspend. The driver can choose to buffer or drop its data."""
            pass

        def suspended(self):
            """Return True when driver is in suspended state."""
            pass

        def shutdown(self):
            """Shutdown the driver"""
            pass

        def setBatchSize(self, size = 0):
            """Can be used by both input and output. A size of zero indicates no batching"""
            pass

        def getBatchSize(self):
            pass

        def canBatch(self):
            """Returns bool. Useful for App to know, if this driver can perform batching at all."""
            pass

        def getFlushInterval(self):
            pass

        def setFlushInterval(self):
            pass

        def subscribeStats(self, callback=None):
            pass

        def onNewStream(self, newStreamName, streamSchema):
            pass


class InputDataDriver(DataDriver):
    pass

class OutputDataDriver(DataDriver):
    def onData(self, streamName, data):
        pass


def load_plugin(pname):
    print "Loading plugin %s" % (pname, )

    path = os.path.dirname(pname)
    # Change the current directory to that of the module. It's not safe to just
    # add the modules directory to sys.path, as that won't accept unicode paths
    # on Windows
    oldpath = os.getcwdu()
    os.chdir(path)

    #print "old path: %s newpath: %s" % (oldpath, path)
    
    modulename, ext = os.path.splitext(os.path.basename(pname))

    if modulename in sys.modules:
        print "unloading %s " % (modulename, )
        #unload_module(sys.modules[modulename])

    print "looking for module %s: " % (modulename, )
    m_info = imp.find_module(modulename, ["."])
    print "module info %s " % repr(m_info, )
    m = imp.load_module(modulename, *m_info)
    print "module loaded"
    
    # Restore the current directory
    os.chdir(oldpath)
    #print "old path %s " % (oldpath, )

    module_plugins = []
    for type_name in dir(m):
        try:
            c = m.__dict__[type_name]            
            if c.__bases__:
                print "name: %s bases: %s " % (c, repr(c.__bases__))
                is_plugin = False
                if issubclass(c, InputDataDriver):
                    inputdata_op_classes.append(c)
                    is_plugin = True
                if issubclass(c, OutputDataDriver):
                    print "Found output data class"
                    outputdata_op_classes.append(c)
                    is_plugin = True
                else:
                    print "not a subclass of OutputDataDriver"

                if is_plugin:
                    module_plugins.append(c)

                
                #TODO: event registration if any
                    
        except AttributeError:
            #TODO: log
            pass

    if(len(module_plugins) > 0):
        m.plugins = module_plugins
        
        

        

class SpringBoardError(Exception):
        def __init__(self, message):
                self.message = message

        def __str__(self):
                return "[ERROR] %s\n" % str(self.message)


"""SpringBoardDataManager API is available for custom plugins
"""


"""
class SpringBoardDataManager(object):

        def __init__(self):
                raise SpringBoardError("Static class")
        
        def log_message(self, data):
            pass

        def version(self):
            pass

        def error_message(self, sender, data):
            pass

        def getAllMarketNames(self):
            pass

        def getAllStreams(self, market="default"):
            pass

        def getSchemaForStream(self, streamName):
            pass

        def getInputDataDriver(self, name):
            pass        

        def getOutputDataDriver(self, name):
            pass
                
        def getStatsForDataDriver(self, name):
            pass
                
        def setNewStream(self, stream, schema):
            pass

        def register(self, streamName, filter, driver):
            #A driver can call register for providing data anytime (during driver initialization time or responding to 
            #    a event - a new webstream being created etc) or the App can register for a driver it knows already for data.
            #    Can be used for bidirectional purposes - input and output
            

            pass

        def unregister(self, streamname, filter, driver):
            pass        

        def onData(self, streamName, driver, data):
            #Called by the InputDriver when it has data to be pumped in.
            pass
"""

            
class SpringBoardDataEventsManager(object):
        """Should contain all events that can be used by drivers and the app likewise. 
        For example, when new data comes in from the driver, we can draw a nice glow line, indicating a data flow.
        """
        pass





def create_input_datadrivers2():
    drivers = [101]
    return drivers

def create_input_datadrivers():
    drivers = []
    for c in inputdata_op_classes:
        drivers.append(c())
    return drivers

def create_output_datadrivers():
    drivers = []
    for c in outputdata_op_classes:
        drivers.append(c())
    print "number of plugins : %d " % (len(drivers),)
    return drivers

def plugin_bootstrapper():
    load_plugin("C:\\Amit\\dev\\cpp\\of_v0.7.4_vs2010_release\\of_v0.7.4_vs2010_release\\apps\\myApps\\alpha-goldDust\\bin\\data\\packages\\testPlugin_DataDrivers.py")


def main() :
    print "load successful"
    #load_plugin("C:\\Amit\\dev\\cpp\\of_v0.7.4_vs2010_release\\of_v0.7.4_vs2010_release\\apps\\myApps\\alpha-goldDust\\bin\\data\\packages\springboard.py")
    plugin_bootstrapper()

if __name__ == '__main__':
    main()

