
import os, sys 
basePlugPath = os.path.join("..", "..")
sys.path.insert(0, os.path.join(basePlugPath, "api"))
sys.path.insert(0, os.path.join(basePlugPath, "external"))

# TODO - know difference between module import vs package import?
import drawingboard
from pattern.web import Twitter, hashtags

import pprint
pp = pprint.PrettyPrinter(indent=4)
print sys.modules["pattern.web"]
#print sys.modules["DataCommand"]
#pp.pprint(sys.modules)

class TwitterDataCommand(drawingboard.DataCommand):
	
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

		# we need to do this automatic - in the sense, identify a plugin, with a given name of plugin Class.
		# simple and one less thing to do for clien api.
		# but we should also provide the below override
		self.set_icon("GUI/icons/twitter.png")
		self.set_display_name("twitter") # todo. to be called automatically by the loading api
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
		#self.set_display_name(self, kwargs.get("name", "twitter"))
		self.args["engine"] = Twitter(language="en")
						
	def set_params(self, **kwargs):
		pass
		
	def get_params(self, **kwargs):
		pass

	#def submit_command(self, port, **commandArgs):
	def execute(self, **commandArgs):
		searchString = commandArgs.get("search", "is cooler than") #from:decebel (from:username is also supported)
		print("searching for {0}: ").format(searchString)
		count = commandArgs.get("count", 25)
		cached = commandArgs.get("cached", False)
		return "skip skip "

		# TODO: arg to decide if we want to do blocking or non-blocking 
		# also, we should allow user to cancel pending requests
		# also, we should auto-alert if the request is taking a long time
		# TODO: we should have test cases for verifying these
		engine = self.args["engine"]
		result = {}
		for tweet in engine.search(searchString, count=count, cached=cached):
			tid = str(hash(tweet.author + tweet.date)) #  since the id is sometimes negative. TODO
			print "# {2} Date={0}. Author= {1}".format(tweet.date, tweet.author, tid)			
			rec = {"author" : tweet.author, "text" : tweet.text, "date" : tweet.date, "hashtags" : hashtags(tweet.text)}
			#print "record = {0}".format(rec)
			result[tweet.author] = rec
			#print "I AM HERE"
			#print "\nRESULT={0}".format(result)
			#print "RECORD = {0}".format(rec)
			#print "HOW MANY TIMES"
			#pp.pprint(result)

			#print tweet.text
		pp.pprint(result)	
		print "FOR LOOP COMPLETE"

		return result

		
		
def main():
	tw = TwitterDataCommand(dummy="dummy")	
	tw.start(en="english")
	res = tw.execute(search="keep your focus") 
	#for k in res:
	#	print "key={0}".format(k)
	pp.pprint(res)




if __name__ == '__main__':
	main()














