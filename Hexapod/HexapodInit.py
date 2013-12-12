import sys, time, serial

class initHandler:

    #Defaults
    baud = 9600     #set baud rate
    timeout = 5     #in seconds
    
    def __init__(self, proj, comPort):
        """
        TODO: Do any initialization that needs to occur before other handlers are
        instantiated.  This also includes the opening of any sockets that need to be
        shared between handlers.

        comPort (string): The comport to connect to (default="COM5")
        """

        self.hexapodSer = None   #serial port for hexapod

        #connect with hexapod
        try:
            self.hexapodSer = serial.Serial(port = comPort, baudrate =
                                           self.baud, timeout = self.timeout)
            #self.hexapodSer.write("h")     #print "h" for debugging purposes
            self.hexapodSer.flush()
            
            # Wait for "setup complete"
            init_response = self.hexapodSer.read()
            print "1st init: ", repr(init_response)

        except:
            print ("(INIT) ERROR: Couldn't connect to hexapod")
            exit(-1)

    #stop connection between LTLMoP and hexapod
    def _stop(self):
        print "(INIT) Shutting down serial port!"
        self.hexapodSer.close()
  
    def getSharedData(self):
        # TODO: Return a dictionary of any objects that will need to be shared with
        # other handlers
        return {"hexapodSer":self.hexapodSer}

