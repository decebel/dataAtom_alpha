
import os, sys 
basePlugPath = os.path.join("..", "..")
sys.path.insert(0, os.path.join(basePlugPath, "api"))
sys.path.insert(0, os.path.join(basePlugPath, "external"))

# TODO - know difference between module import vs package import?
import drawingboard
from pattern.web import Wikipedia

import pprint
pp = pprint.PrettyPrinter(indent=4)
print sys.modules["pattern.web"]
#print sys.modules["DataCommand"]
#pp.pprint(sys.modules)

class WikipediaDataCommand(drawingboard.DataCommand):
	
	def __init__(self, **kwargs):
		drawingboard.DataCommand.__init__(self, **kwargs)
		print "init called "
		#self.drawingboard.DataCommand
		self.args = {}

	def load_resources(self, **kwargs):
		"""sets initial status of loading icon. then loads the icon. then sets various other things and 
		as it does this, it will keep calling status message. 
		"""
		print "loading "
		#self.set_icon("GUI/icons/blogger.png")

		# trying to figure out the icon to use based on display name
		self.set_display_name("blogger")
		self.set_initialized(True)


	#def start(self, **kwargs):
	def start(self, **kwargs):
		"""Configures the command.

		- sets the display name of the command
		- sets initial status string
		- sets a default icon - NO. Default Icon setup should happen well before this stage. Maybe a load api.
		- sets is_initialized to return true, once all is well. TODO: Should we check for a net connection?
		
		Note: all these arguments can also be set by callin set_params with key=value pairs.
		is_initialized will return true when all the required argum(ents are ready
		"""
		#self.set_display_name(self, kwargs.get("name", "wikipedia"))
		self.args["engine"] = Wikipedia(language="en")
						
	def set_params(self, **kwargs):
		pass
		
	def get_params(self, **kwargs):
		pass

	#def submit_command(self, port, **commandArgs):
	def execute(self, **commandArgs):
		searchString = commandArgs.get("search", "life of pi") #from:decebel (from:username is also supported)
		print("searching for {0}: ").format(searchString)
		timeout = commandArgs.get("timeout", 25)
		cached = commandArgs.get("cached", False)
		engine = self.args["engine"]
		return "skip skip"
		article = engine.search(searchString, cached=cached, timeout=timeout)

		print article.title

		for s in article.sections:
			print s.title.upper()
			print 
			print s.content
			print

		return article.title

	
def main():
	wp = WikipediaDataCommand(dummy="dummy")	
	wp.start(en="english")
	res = wp.execute(search="Like of Pi") 
	#for k in res:
	#	print "key={0}".format(k)
	pp.pprint(res)




if __name__ == '__main__':
	main()














