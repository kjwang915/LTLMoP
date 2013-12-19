import time

class hexapodActuatorHandler:
    def __init__(self, proj, shared_data):
    """
    Connect to hexapod
    """
        try:
            self.hexapodSer = shared_data["hexapodSer"]
        except:
            logging.exception("no connection to hexapod")
            sys.exit()

    def _sendCommand(self, cmd):
       """
       Send locomotion command ``cmd`` to the robot
       """

       #print action being sent
       if cmd == 'e':
           logging.debug("sending standUp")
       elif cmd == 'f':
           logging.debug("sending sitDown")
       elif cmd == 'g':
           logging.debug("sending tailUp")


       
       #send command to Arduino on hexapod
       self.hexapodSer.write(cmd)

       #wait for a response from Arduino
       x = self.hexapodSer.read()

       #if Arduino does not respond, print "received invalid"
       #waits until Arduino responds
       while x != 'q':
           logging.debug("received invalid")
           self.hexapodSer.write(cmd)
           x = self.hexapodSer.read()
       


    def standUp(self, actuatorVal, initial=False):
        """
        stand up action
        """
        if initial:
            return
        if actuatorVal:
            #sends character 'e' for hexapod to stand up
            init.sendCommand('e')


    def sitDown(self, actuatorVal, initial=False):
        """
        sit down action
        """
        if initial:
            return
        if actuatorVal:
            #sends character 'f' for hexapod to sit down
            init.sendCommand('f')


    def tailUp(self, actuatorVal, initial=False):
        """
        tail up action
        """
        if initial:
            return
        if actuatorVal:
            #sends character 'g' for hexapod to put tail up
            init.sendCommand('g')
