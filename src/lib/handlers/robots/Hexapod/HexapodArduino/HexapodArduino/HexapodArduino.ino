/*
This is the code that is uploaded onto the Arduino Uno board that is mounted onto the hexapod.
This code controls the different movements of the hexapod by sending servo position values to the
SSC-32 servo controller board that is mounted onto the hexapod.

Kevin J. Wang
*/

//include this library to allow for extra serial communication ports on the Arduino
#include <SoftwareSerial.h>

/*
The values below correspond to different servo position values so that these different positions are
able to be called directly.
These values were found using the sequencer that can be downloaded from the Lynxmotion website.
*/
//neutral position
unsigned int neutral[] = 
  {-1, 1250, 1462, 1420, -1, 1490, 1860, 1540, 
  -1, 1900, 1490, 1750, -1, 2000, 1610, 1520, 
  1200, 1503, 1755, 1460, 1526, 1020, -1, 980,
  -1, 1580, 1100, 1460, -1, 960, 1600, 1450};

//low position
int low[] = 
  {-1, 1250, 1462, 1420, -1, 833, 1019, 833, 
  -1, 1093, 1490, 1290, -1, 1154, 895, 1520, 
  1200, 1503, 1755, 1460, 2265, 1969, -1, 1907, 
  -1, 2340, 1480, 1460, -1, 1824, 2377, 1450};

//high position
int high[] = 
  {-1, 1250, 1462, 1420, -1, 1994, 2412, 1985, 
  -1, 2396, 1490, 1290, -1, 2500, 2068, 1520, 
  1200, 1503, 1755, 1460, 948, 500, -1, 500, 
  -1, 997, 1480, 1460, -1, 500, 1154, 1450};

/*
The matrices below contain the initial and final positions of the leg servo angles.
These values are found from an excel document attached with basic algebra calculations that will provide
you with three values for each leg: phi, alpha, and beta which correspond to the hip, femur, and tibia of
the hexapod's legs, respectively.
These values are in radians. The respective radian values for the servo angle positions
were found by mapping the maximum and minimum servo position values to 0 and 2*pi in radians.
*/

float rightStart[9] = {
 0.883978677, -0.227081937, PI/2 -1.825051286, 
0.631690634, 0.127224656, PI/2 -1.517417323, 
 -0.2392316, 0.213516457,   PI/2 -1.363522667};
 
float rightEnd[9] = {
 0.350727948, 0.203412118, PI/2 -1.387802641, 
 -0.453844002, 0.187072201, PI/2 -1.42161556, 
 -0.831876295, -0.08484119, PI/2 -1.732514015};

float leftStart[9];
float leftEnd[9];

//define the new serial ports for communication
SoftwareSerial Servos(10, 11); // RX, TX

//The mapping of each servo.
//The first value in each row corresponds to its position on the leg (top, middle, bottom)
//The value itself corresponds to the pin to which it is connected on the SSC-32 servo controller board
 const float leftTable[9] = {
    3, 6, 5,                  //left front leg
    10, 9, 7,                 //left middle leg 
    11, 13, 14 };             //left back leg
    
  const float rightTable[9] = {
    19, 21, 20,               //right front leg
    27, 23, 25,               //right middle leg
    26, 29, 30  };            //right back leg

//The matrices below are the initial values of the servo positions of the legs
//These values will be added to the offset values, allowing for movement of the legs
  const int leftneutral_val[9] = {
    1883, 1860, 1574,         //left front leg
    1512, 1900, 1540,         //left middle leg
    957, 2000, 1747 };        //left back leg  
  
  const int rightneutral_val[9] = {
    1006, 1040, 1438,         //right front leg
    1460, 1056, 1488,         //right middle leg
    1846, 1080, 1590 };       //right back leg

//setup Arduino board
void setup() {
  Serial.begin(9600);                //set baud rate for XBees
  Servos.begin(9600);                //set baud rate for serial communication between Arduino and SSC-32
  for (int i = 0; i <= 31; i++){     //set hexapod to neutral position
    moveServos(i, neutral[i],0.5);
  }
}

/*
Wait until a command is received. When a command is received, perform an action
*/
void loop() {

  // flush the serial rx buffer
  while (Serial.available())
      Serial.read();
  
   
  while(!Serial.available());
    int inByte = Serial.read();    //read from serial, take a character
    
  switch (inByte) {
      case 'a':  //forwards
      
      //set left leg to be mirror of right leg
       for (int i = 0; i < 9; i++) {
          leftStart[i] = -rightStart[i];
          leftEnd[i] = -rightEnd[i];
        }
        forward();            //walk forwards
        Serial.write('q');        //write back through serial when action is complete
        Serial.flush();           //waits for transmission of outgoing serial data to complete
        break;
      case 'b':  //neutral position
        for (int i = 0; i <= 31; i++){
          moveServos(i, neutral[i],0.5);
        }
        Serial.write('q');
        Serial.flush();
        break;
      case 'c':  //clockwise turn
      
      //set the start angles of the hip of the left leg to the end hip values of the right leg
      //and set the end values of the hip of the left leg start value of the hips of the right legs
      //left leg will push backwards and right leg forwards
       for (int i = 0; i < 3; i++) {
          leftStart[i*3] = rightEnd[i*3];
          leftEnd[i*3] = rightStart[i*3];
        }
        turn();
        Serial.write('q');
        Serial.flush();

        break;
      case 'd':  //counterclockwise turn
      
        //left leg will push forwards and right leg backwards
        for (int i = 0; i < 3; i++) {
          leftStart[i*3] = -rightEnd[i*3];
          leftEnd[i*3] = -rightStart[i*3];
        }
        turn();
        Serial.write('q');
        Serial.flush();

        break;
      
      case 'e':  //stand up
        for (int i = 0; i <= 31; i++){
          sendPosition(i, high[i],0.5);
        }
        Serial.write('q');
        Serial.flush();
        break;
      case 'f':  //sit down
        for (int i = 0; i <= 31; i++){
          sendPosition(i, low[i],0.5);
        }
        Serial.write('q');
        Serial.flush();
        break;
      case 'g':  //move tail
        move(15,2000,10);
        Serial.write('q');
        Serial.flush();
        break;

    }
}

/*
function to turn; gait is slightly different from forwards gait
gait moves two legs at a time, thus there are 3 PODs
*/
void turn() {
  
  //set tail and head to neutral position so they do not affect movement
  moveServos(3,1420,1);
  moveServos(5,1490,1);
  moveServos(6,1860,1);
  moveServos(7,1540,1);
  moveServos(9,1900,1);
  moveServos(10,1490,1);
  moveServos(11,1290,1);
  moveServos(13,2000,1);
  moveServos(14,1200,1); 
  
  //POD 1 go to beginning
  servoSerial(0, rightStart[0], 50, true);
  servoSerial(1, rightEnd[1] + PI/8, 10, true);
  servoSerial(2, rightEnd[2] - PI/8, 10, true);
  
  servoSerial(6, leftStart[6], 50, false);
  servoSerial(7, leftEnd[7] - PI/2, 10, false);
  servoSerial(8, leftEnd[8] + PI/2, 10, false);
  //POD 1
  servoSerial(0, rightStart[0], 10, true);
  servoSerial(1, rightStart[1], 10, true);
  servoSerial(2, rightStart[2], 10, true);
  
  servoSerial(6, leftStart[6], 10, false);
  servoSerial(7, leftStart[7], 10, false);
  servoSerial(8, leftStart[8], 10, false);
  
  //POD 1 moving
  servoSerial(0, rightEnd[0], 1000, true);
  servoSerial(1, rightEnd[1], 1000, true);
  servoSerial(2, rightEnd[2], 1000, true);
  
  servoSerial(6, leftEnd[6], 1000, false);
  servoSerial(7, leftEnd[7], 1000, false);
  servoSerial(8, leftEnd[8], 1000, false);
  
  //delay(100);
  //POD 2 to beginning
  servoSerial(3, rightStart[3], 50, true);
  servoSerial(4, rightEnd[4] + PI/8, 10, true);
  servoSerial(5, rightEnd[5] - PI/8, 10, true);
  
  servoSerial(0, leftStart[0], 50, false);
  servoSerial(1, leftEnd[1] - PI/8, 10, false);
  servoSerial(2, leftEnd[2] + PI/8, 10, false);
  
  //POD 2
  servoSerial(3, rightStart[3], 10, true);
  servoSerial(4, rightStart[4], 10, true);
  servoSerial(5, rightStart[5], 10, true);
  
  servoSerial(0, leftStart[0], 10, false);
  servoSerial(1, leftStart[1], 10, false);
  servoSerial(2, leftStart[2], 10, false);
  
  //POD 2 moving
  servoSerial(3, rightEnd[3], 400, true);
  servoSerial(4, rightEnd[4], 400, true);
  servoSerial(5, rightEnd[5], 400, true);
  
  servoSerial(0, leftEnd[0], 1000, false);
  servoSerial(1, leftEnd[1], 1000, false);
  servoSerial(2, leftEnd[2], 1000, false);
  
  //delay(100);
  
  //POD 3 to beginning 
  servoSerial(6, rightStart[6], 50, true);
  servoSerial(7, rightEnd[7] + PI/6, 10, true);
  servoSerial(8, rightEnd[8] - PI/6, 10, true);
  
  servoSerial(3, leftStart[3], 50, false);
  servoSerial(4, leftEnd[4] - PI/8, 10, false);
  servoSerial(5, leftEnd[5] + PI/8, 10, false);
  
  //POD 3
  servoSerial(6, rightStart[6], 10, true);
  servoSerial(7, rightStart[7], 10, true);
  servoSerial(8, rightStart[8], 10, true);
  
  servoSerial(3, leftStart[3], 10, false);
  servoSerial(4, leftStart[4], 10, false);
  servoSerial(5, leftStart[5], 10, false);
  
  //POD 3 moving
  servoSerial(6, rightEnd[6], 1000, true);
  servoSerial(7, rightEnd[7], 1000, true);
  servoSerial(8, rightEnd[8], 1000, true);
  
  servoSerial(3, leftEnd[3], 500, false);
  servoSerial(4, leftEnd[4], 500, false);
  servoSerial(5, leftEnd[5], 500, false);
}

/*
this gait works better than previous gait when only walking forwards
gait moves two legs at a time, thus there are 3 PODs
*/
void forward() {
  //POD 1 go to beginning
  servoSerial(0, rightEnd[0], 50, true);
  servoSerial(1, rightEnd[1] + PI/10, 20, true);
  servoSerial(2, rightEnd[2] - PI/10, 20, true);
  
  servoSerial(6, leftEnd[6], 20, false);
  servoSerial(7, leftEnd[7] - PI/10, 20, false);
  servoSerial(8, leftEnd[8] + PI/10, 20, false);
  //POD 1
  servoSerial(0, rightStart[0], 20, true);
  servoSerial(1, rightStart[1], 20, true);
  servoSerial(2, rightStart[2], 20, true);
  
  servoSerial(6, leftStart[6], 20, false);
  servoSerial(7, leftStart[7], 20, false);
  servoSerial(8, leftStart[8], 20, false);
  
  //POD 1 moving
  servoSerial(0, rightEnd[0], 100, true);
  servoSerial(1, rightEnd[1], 100, true);
  servoSerial(2, rightEnd[2], 100, true);
  
  servoSerial(6, leftEnd[6], 100, false);
  servoSerial(7, leftEnd[7], 100, false);
  servoSerial(8, leftEnd[8], 100, false);
  
  //delay(220);
  //POD 2 to beginning
  servoSerial(3, rightEnd[3], 20, true);
  servoSerial(4, rightEnd[4] + PI/10, 20, true);
  servoSerial(5, rightEnd[5] - PI/10, 20, true);
  
  servoSerial(0, leftEnd[0], 20, false);
  servoSerial(1, leftEnd[1] - PI/10, 20, false);
  servoSerial(2, leftEnd[2] + PI/10, 20, false);
  
  //POD 2
  servoSerial(3, rightStart[3], 20, true);
  servoSerial(4, rightStart[4], 20, true);
  servoSerial(5, rightStart[5], 20, true);
  
  servoSerial(0, leftStart[0], 20, false);
  servoSerial(1, leftStart[1], 20, false);
  servoSerial(2, leftStart[2], 20, false);
  
  //POD 2 moving
  servoSerial(3, rightEnd[3], 200, true);
  servoSerial(4, rightEnd[4], 200, true);
  servoSerial(5, rightEnd[5], 200, true);
  
  servoSerial(0, leftEnd[0], 200, false);
  servoSerial(1, leftEnd[1], 200, false);
  servoSerial(2, leftEnd[2], 200, false);
  
  //delay(220);
  
  //POD 3 to beginning 
  servoSerial(6, rightEnd[6], 20, true);
  servoSerial(7, rightEnd[7] + PI/10, 20, true);
  servoSerial(8, rightEnd[8] - PI/10, 20, true);
  
  servoSerial(3, leftEnd[3], 20, false);
  servoSerial(4, leftEnd[4] - PI/10, 20, false);
  servoSerial(5, leftEnd[5] + PI/10, 20, false);
  
  //POD 3
  servoSerial(6, rightStart[6], 20, true);
  servoSerial(7, rightStart[7], 20, true);
  servoSerial(8, rightStart[8], 20, true);
  
  servoSerial(3, leftStart[3], 20, false);
  servoSerial(4, leftStart[4], 20, false);
  servoSerial(5, leftStart[5], 20, false);
  
  //POD 3 moving
  servoSerial(6, rightEnd[6], 200, true);
  servoSerial(7, rightEnd[7], 200, true);
  servoSerial(8, rightEnd[8], 200, true);
  
  servoSerial(3, leftEnd[3], 200, false);
  servoSerial(4, leftEnd[4], 200, false);
  servoSerial(5, leftEnd[5], 200, false);
}

/*
adds neutral value offsets and then sends the servo positions through serial to the SSC-32
*/
void servoSerial(int ser, float pos, int time, boolean right) {
  
  //this is used to change from radians to servo position angles
  //found this relationship by mapping the max and min values of the servo to radians on excel
  pos = pos * 180/PI;
  pos = 11.111*pos;
  
  if (right) {
    pos = pos+rightneutral_val[ser];
    ser = rightTable[ser];
  }
  else {
    pos = pos+leftneutral_val[ser];
    ser = leftTable[ser];
  }
  Servos.print('#');
  Servos.print(ser);
  Servos.print(" P");
  Servos.print((int)pos);
  Servos.print(" T");
  Servos.println(time);
}

/*
simple function that allows for input of each specific servo position directly (no offset values)
*/
void sendPosition(int servo, int pos, int time){
  Servos.print("#");
  Servos.print(servo);
  Servos.print(" P");
  Servos.print(pos);
  Servos.print(" T");
  Servos.println(time);
  
}
