import sys, time, serial

class hexapodInitHandler:

    #Defaults
    baud = 9600     #set baud rate
    timeout = 5     #in seconds
    
    def __init__(self, proj, comPort):
        """
        Connects to hexapod
        
        comPort (string): The comport to connect to (default="COM5")
        """

        self.hexapodSer = None   #serial port for hexapod

        #connect with hexapod
        try:
            self.hexapodSer = serial.Serial(port = comPort, baudrate =
                                           self.baud, timeout = self.timeout)
            
            # Wait for "setup complete"
            init_response = self.hexapodSer.read()
            print "1st init: ", repr(init_response)

        except:
            logging.exception("could not connect to hexapod")
            sys.exit()


    def _stop(self):
    """
    stop connection between LTLMoP and hexapod
    """
        logging.info("shutting down serial port")
        self.hexapodSer.close()
  
    def getSharedData(self):
    """
    Return a dictionary of any objects that will need to be shared with other handlers
    """

        return {"hexapodSer":self.hexapodSer}
        
    def sendCommand(self, cmd):
        """
        Send locomotion command ``cmd`` to the robot
        """
        

        #print action being sent
        if cmd == 'a':
            logging.debug("sending move")
        elif cmd == 'b':
            logging.debug("sending neutral")
        elif cmd == 'c':
            logging.debug("sending clockwise")
        elif cmd == 'd':
            logging.debug("sending counterclockwise")
        elif cmd == 'e':
            logging.debug("sending standUp")
        elif cmd == 'f':
            logging.debug("sending sitDown")
        elif cmd == 'g':
            logging.debug("sending tailUp")


        #send command to Arduino on hexapod
        self.hexapodSer.write(cmd)

        #wait for response from Arduino
        x = self.hexapodSer.read()
        while x != 'q':
            logging.debug("received invalid")
            self.hexapodSer.write(cmd)
            x = self.hexapodSer.read()

