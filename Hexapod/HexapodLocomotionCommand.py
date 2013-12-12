#!/usr/bin/env python
# -*- coding: cp1252 -*-
"""
================================================================================
HexapodLocomotionCommand.py - The Hexapod's Locomotion Command Handler
================================================================================
"""

import time

class locomotionCommandHandler:     
    def __init__(self, proj, shared_data):
       # connect with hexapod
       try:
            self.hexapodSer = shared_data["hexapodSer"]
       except:
            print "(LOCO) ERROR: No connection to  hexapod"
            exit(-1)
    
    def sendCommand(self, cmd):
        # Send locomotion command ``cmd`` to the robot

        #print action being sent
        if cmd == 'a':
            print 'sending move'
        elif cmd == 'b':
            print 'sending neutral'
        elif cmd == 'c':
            print 'sending clockwise'
        elif cmd == 'd':
            print 'sending counterclockwise'

        #print 'sending, ',cmd     #used for debugging purposes

        #send command to Arduino on hexapod
        self.hexapodSer.write(cmd)

        #wait for response from Arduino
        x = self.hexapodSer.read()
        while x != 'q':
            print "received invalid ack: ", repr(x)
            self.hexapodSer.write(cmd)
            x = self.hexapodSer.read()

    def move(self, distance):
        """
        tells robot to move
        """
        self.sendCommand('a')

    def stop(self):
        """
        tells robot to go to neutral position
        """
        self.sendCommand('b')

    def turnClockwise(self, angle):
        """
        tells robot to turn
        """
        self.sendCommand('c')
        
    def turnCounterclockwise(self, angle):
        """
        tells robot to turn
        """
        self.sendCommand('d')


