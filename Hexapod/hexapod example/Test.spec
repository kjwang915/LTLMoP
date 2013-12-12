# This is a specification definition file for the LTLMoP toolkit.
# Format details are described at the beginning of each section below.


======== SETTINGS ========

Actions: # List of action propositions and their state (enabled = 1, disabled = 0)
standUp, 1
sitDown, 1

CompileOptions:
convexify: True
parser: structured
fastslow: False
decompose: True
use_region_bit_encoding: True

CurrentConfigName:
Simulation

Customs: # List of custom propositions

RegionFile: # Relative path of region description file
boxes_original_fixed.regions

Sensors: # List of sensor propositions and their state (enabled = 1, disabled = 0)


======== SPECIFICATION ========

RegionMapping: # Mapping between region names and their decomposed counterparts
black = p5
green = p3
red = p2
others = p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21

Spec: # Specification in structured English
#Robot starts in black
Robot starts with false
go to red
if you are in red then do standUp

go to green
if you are in green then do sitDown

