#include <AFMotor.h>

#define TURN_LEFT 0
#define TURN_RIGHT 1
#define GO_FORWARD 2
#define GO_BACKWARD 3

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
static int beaconLeft = 1;
static int beaconRight = 2;
static int tape = 3;
static int state;



//takes in between -255 and 255
void leftSpeed(int speed)
{
  motor1.setSpeed(abs(speed));
  motor2.setSpeed(abs(speed));
  if(speed > 0)
  {
     motor1.run(FORWARD);
     motor2.run(FORWARD);
  }
  else
  {
     motor1.run(BACKWARD);
     motor2.run(BACKWARD);    
  }
}

void rightSpeed(int speed)
{
  motor3.setSpeed(abs(speed));
  motor4.setSpeed(abs(speed));
   if(speed > 0)
   {
     motor3.run(FORWARD);
     motor4.run(FORWARD);
   } 
   else
   {
     motor3.run(BACKWARD);
     motor4.run(BACKWARD);
   }
}

void turnRight()
{
  leftSpeed(255);
  rightSpeed(-255);
  state = TURN_RIGHT;
}

void turnLeft()
{
  leftSpeed(-255);
  rightSpeed(255);
  state = TURN_LEFT;
}

void goForward()
{
  leftSpeed(255);
  rightSpeed(255);
  state = GO_FORWARD;  
}

void goBackward()
{
  leftSpeed(-255);
  rightSpeed(-255);
  state = GO_BACKWARD;    
}

void respToTape()
{
  if(state == GO_FORWARD)
  {
    goBackward();
  }
  else goForward();
}

void setup() {
  Serial.begin(9600);
  pinMode(beaconLeft,INPUT);
  pinMode(beaconRight,INPUT);
  pinMode(tape,INPUT);
  goForward();
}


void loop()
{
   if(HIGH == digitalRead(tape))
   {
      respToTape();
   }
   else if(HIGH == digitalRead(beaconLeft) && HIGH == digitalRead(beaconRight))
   {
       goForward();
   }
   else if(HIGH == digitalRead(beaconLeft))
   {
      turnLeft();
   }
   else if(HIGH == digitalRead(beaconRight))
   {
       turnRight();
   }
   else //both beaconLeft and beaconRight are LOW
   {
      turnLeft(); 
   }
}
