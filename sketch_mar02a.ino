#include <Timers.h>
#include <AFMotor.h>


#define SEARCH 0
#define ATTACK 1
#define SLIGHT_LEFT 2
#define SLIGHT_RIGHT 3
#define BACK_UP 4
#define FAST 255
#define SLOW 150
#define ONE_SEC 1000

AF_DCMotor motor1(1,MOTOR12_64KHZ);
AF_DCMotor motor2(2,MOTOR12_64KHZ);
AF_DCMotor motor3(3,MOTOR12_64KHZ);
AF_DCMotor motor4(4,MOTOR12_64KHZ);

static int beaconLeft = 30;
static int beaconRight = 34;
static int tape = 38;
static int state = SEARCH;

void setup() {
  Serial.begin(9600);
  pinMode(beaconLeft,INPUT);
  pinMode(beaconRight,INPUT);
  pinMode(tape,INPUT);
  attack();
  TMRArd_InitTimer(3,120*ONE_SEC);
}

void loop(){
  if(TMRArd_IsTimerExpired(3) == TMRArd_EXPIRED){
    leftSpeed(0);
    rightSpeed(0);
  }
  else switch(state){ //switches between different states
    case (SEARCH): //spinning left
      if(tapeSensor()) backUp();
      else if(bothBeacons()) attack();
      else if(leftBeacon()) slightLeft();
      else if(rightBeacon() || (TMRArd_IsTimerExpired(0) == TMRArd_EXPIRED)) slightRight();
      break;
    case (ATTACK): //full speed ahead
      if(tapeSensor()) backUp();
      else if(leftBeacon()) slightLeft();
      else if(rightBeacon()) slightRight();
      break;
    case (SLIGHT_LEFT): //left motors slightly slower
      if(tapeSensor())backUp();
      else if(bothBeacons())attack();
      else if(rightBeacon())slightRight();
      break;
    case (SLIGHT_RIGHT): //right motors slightly slower
      if(tapeSensor())backUp();
      else if(bothBeacons())attack();
      else if(leftBeacon())slightLeft();
      break;
    case (BACK_UP): //full speed backward
      if(tapeSensor() && (TMRArd_IsTimerExpired(2) == TMRArd_EXPIRED))slightRight();
      else if(TMRArd_IsTimerExpired(1) == TMRArd_EXPIRED) search();
      break;
  }
}

//takes in between -255 and 255
void leftSpeed(int speed){
  motor1.setSpeed(abs(speed));
  motor2.setSpeed(abs(speed));
  if(speed > 0){
     motor1.run(FORWARD);
     motor2.run(FORWARD);
  }
  else{
     motor1.run(BACKWARD);
     motor2.run(BACKWARD);    
  }
}

void rightSpeed(int speed){
  motor3.setSpeed(abs(speed));
  motor4.setSpeed(abs(speed));
   if(speed > 0){
     motor3.run(FORWARD);
     motor4.run(FORWARD);
   } 
   else{
     motor3.run(BACKWARD);
     motor4.run(BACKWARD);
   }
}

void slightRight(){
  leftSpeed(FAST);
  rightSpeed(SLOW);
  state = SLIGHT_RIGHT;
} 

void slightLeft(){
  leftSpeed(SLOW);
  rightSpeed(FAST);
  state = SLIGHT_LEFT;
}

void search(){
  leftSpeed(-175);
  rightSpeed(175);
  state = SEARCH;
  TMRArd_InitTimer(0,6*ONE_SEC);
}

void attack(){
  leftSpeed(FAST);
  rightSpeed(FAST);
  state = ATTACK;
}

void backUp(){
  leftSpeed(-FAST);
  rightSpeed(-FAST);  
  state = BACK_UP;
  TMRArd_InitTimer(1,1*ONE_SEC);
  TMRArd_InitTimer(2,ONE_SEC/10);
}

int bothBeacons(){
  return ((digitalRead(beaconLeft) == HIGH) && (digitalRead(beaconRight) ==  HIGH));
}

int leftBeacon(){
  return ((digitalRead(beaconLeft) == HIGH) && (digitalRead(beaconRight) !=  HIGH));
}

int rightBeacon(){
  return ((digitalRead(beaconLeft) != HIGH) && (digitalRead(beaconRight) ==  HIGH));
}

int tapeSensor(){
  return (digitalRead(tape) == HIGH);
}
