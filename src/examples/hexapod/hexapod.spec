# This is a specification definition file for the LTLMoP toolkit.
# Format details are described at the beginning of each section below.


======== SETTINGS ========

Actions: # List of action propositions and their state (enabled = 1, disabled = 0)
standUp, 1
sitDown, 1
gripperOpen, 1
nod, 1
shake, 1

CompileOptions:
convexify: True
parser: structured
symbolic: False
use_region_bit_encoding: True
synthesizer: jtlv
fastslow: False
decompose: True

CurrentConfigName:
Untitled configuration

Customs: # List of custom propositions

RegionFile: # Relative path of region description file
lab_map.regions

Sensors: # List of sensor propositions and their state (enabled = 1, disabled = 0)
item, 1


======== SPECIFICATION ========

RegionMapping: # Mapping between region names and their decomposed counterparts
r1 = p6, p7
r2 = p2
r3 = p8, p9
Upload = p4
Charge = p5
others = 

Spec: # Specification in structured English
robot starts with not gripperOpen

if you are sensing item then visit r2
if you are not sensing item then visit Charge

# open the gripper if it was closed and nothing is picked
if you are in Charge and you are not sensing item and you were not activating gripperOpen then do gripperOpen

if you are in Charge and you were activating gripperOpen then do not gripperOpen

if you are not in r2 and you are sensing item then do not gripperOpen

if you are in r2 and you are sensing item then do gripperOpen and nod

