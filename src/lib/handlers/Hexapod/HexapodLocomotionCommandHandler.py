#!/usr/bin/env python
# -*- coding: cp1252 -*-
"""
================================================================================
HexapodLocomotionCommandHandler.py - The Hexapod's Locomotion Command Handler
================================================================================
"""

import lib.handlers.handlerTemplates as handlerTemplates

class HexapodLocomotionCommandHandler(handlerTemplates.LocomotionCommandHandler):
    def __init__(self, executor, shared_data):
        """
        Locomotion handler for Hexapod
        """
    
        # get serial port of hexapod
        try:
            self.hexapodSer = shared_data["hexapodSer"]
        except:
            print "(LOCO) ERROR: No connection to  hexapod"
            exit(-1)

    def sendCommand(self, cmd):
        """
        Send locomotion command ``cmd`` to the robot
        """

        self.hexapodSer.write(cmd)

        x = self.hexapodSer.read()
        while x != 'q':
            print "received invalid ack: ", repr(x)
            self.hexapodSer.write(cmd)
            x = self.hexapodSer.read()

    def move(self, distance):
        """
        tells robot to move forward
        """
        self.sendCommand('a')

    def stop(self):
        """
        tells robot to stop
        """
        self.sendCommand('b')

    def turnClockwise(self):
        """
        tells robot to turn
        """      
        self.sendCommand('c')
        
    def turnCounterclockwise(self):
        """
        tells robot to turn
        """
        self.sendCommand('d')


