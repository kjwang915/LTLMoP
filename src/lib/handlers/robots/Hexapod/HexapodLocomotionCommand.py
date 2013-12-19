#!/usr/bin/env python
# -*- coding: cp1252 -*-
"""
================================================================================
HexapodLocomotionCommand.py - The Hexapod's Locomotion Command Handler
================================================================================
"""

import time

class hexapodLocomotionCommandHandler:     
    def __init__(self, proj, shared_data):
       """
       connect with hexapod
       """
       
       try:
            self.hexapodSer = shared_data["hexapodSer"]
       except:
            logging.exception("no connection to hexapod")
            sys.exit()
    


    def move(self, distance):
        """
        tells robot to move
        """
        init.sendCommand('a')

    def stop(self):
        """
        tells robot to go to neutral position
        """
        init.sendCommand('b')

    def turnClockwise(self, angle):
        """
        tells robot to turn
        """
        init.sendCommand('c')
        
    def turnCounterclockwise(self, angle):
        """
        tells robot to turn
        """
        init.sendCommand('d')


