import time

class actuatorHandler:
    def __init__(self, proj, shared_data):
        # Connect to hexapod
        try:
            self.hexapodSer = shared_data["hexapodSer"]
        except:
            print "(ACTUATOR) ERROR: No connection to  hexapod"
            exit(-1)

    def _sendCommand(self, cmd):
       # Send locomotion command ``cmd`` to the robot

       #print action being sent
       if cmd == 'e':
           print 'sending standUp'
       elif cmd == 'f':
           print 'sending sitDown'
       elif cmd == 'g':
           print 'sending tailUp'

       #print 'sending, ',cmd     #used for debugging purposes
       
       #send command to Arduino on hexapod
       self.hexapodSer.write(cmd)

       #wait for a response from Arduino
       x = self.hexapodSer.read()

       #if Arduino does not respond, print "received invalid"
       #waits until Arduino responds
       while x != 'q':
           print "received invalid ack: ", repr(x)
           self.hexapodSer.write(cmd)
           x = self.hexapodSer.read()
       self.hexapodSer.flush()

    #stand up action
    #sends character 'e' for hexapod to stand up
    def standUp(self, actuatorVal, initial=False):
        """
        tells robot to go up
        """
        if initial:
            return
        if actuatorVal:
            self._sendCommand('e')

    #sit down action
    #sends character 'f' for hexapod to sit down
    def sitDown(self, actuatorVal, initial=False):
        """
        tells robot to go down
        """
        if initial:
            return
        if actuatorVal:
            self._sendCommand('f')

    #tail up action
    #sends character 'g' for hexapod to put tail up
    def tailUp(self, actuatorVal, initial=False):
        """
        tells put tail up
        """
        if initial:
            return
        if actuatorVal:
            self._sendCommand('g')
