RobotName: # Robot Name
Bob

Type: # Robot type
Hexapod

ActuatorHandler: # Robot default actuator handler with default argument values
HexapodActuator()

SensorHandler: # Robot default actuator handler with default argument values
HexapodSensor()

DriveHandler: # Robot default drive handler with default argument values
HexapodDrive()

InitHandler: # Robot default init handler with default argument values
HexapodInit(comPort="COM5")

LocomotionCommandHandler: # Robot locomotion command actuator handler with default argument values
HexapodLocomotionCommand()

MotionControlHandler: # Robot default motion control handler with default argument values
vectorController()

PoseHandler: # Robot default pose handler with default argument values
viconPose(host='10.0.0.102',port=800,x_VICON_name="Hexapod:Hexapod <t-X>",y_VICON_name="Hexapod:Hexapod <t-Y>",theta_VICON_name="Hexapod:Hexapod <a-Z>")
