from springboard import OutputDataDriver

class TestOutputDataDriver1(OutputDataDriver):

    def onData(self, streamName, data):
        print "test from onData1: %s" % (data, )


class TestOutputDataDriver2(OutputDataDriver):

    def onData(self, streamName, data):
        print "test from onData2: %s" % (data, )

        
"""
def main():
    c = TestOutputDataDriver1()
    c.onData("teststream", "hello")

if __name__ == "__main__":
    main()
    """
