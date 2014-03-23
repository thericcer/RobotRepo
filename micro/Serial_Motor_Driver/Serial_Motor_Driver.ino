/*
 * Serial motor controller
 * Recieves a serial string formatted as such-
 *   M, SL, SR, DirL, DirR
 *   M, A1, A2, A3, A4 
 * The first byte corresponds to what motor set we are controlling, Steering or Drive.
 * The next four bytes correspond to the speed/direction or angle at which to run/turn the motor. 
 * 
 * Finally the code checks the validty of the string, then sets the appropriate pins
 * to drive the called for motors.
 * 
 * Upon further thinking on steering, the 2 front and 2 rear motors can not be tied
 * to eachother because that would make steering impossible. We must independantly 
 * control each motor seperately.
 *
 * Oct 21 2013
 * Ric Clark
 */

#include <Servo.h>

#define DRIVE 'D'
#define STEER 'S'
#define BREAKOUT 'B'
#define FORWARD 'F'
#define SENSOR 'Z'
#define STATUS 'C'
#define PLATFORM 'P'

enum {ERROR, NORMAL, SLEEP};
char errorFlag = 0;

unsigned char inPacket[5];

/* Define a Status Byte, This will be sent as a response to recieving a 5 byte command packet
 * 
 * 0                       7
 * |B0|B1|B2|B3|B4|B5|B6|B7|
 *
 * B0 - If Set, Motors were Killed by External Interrupt, Action Needs to be taken
 * B1 - Command Received, Values changed
 * More to come
 */
unsigned char StatusByte = 0;
unsigned char LoopCount = 0;

int SensorArray[4] = {0};
int SensorCal[4] = {0};

unsigned char SteerArray[4] = {0};
unsigned char MotorArray[4] = {0};

int loopTime = 0;
int oldLoopTime = 0;

Servo leftFront;
Servo leftRear;
Servo rightFront;
Servo rightRear;
Servo platformLeft;
Servo platformRight;

volatile char state;

void sensorISR(){
  state = ERROR;
}

void setup(){
  //Begin Serial at common baud rate.
  Serial.begin(9600);
  
  /*Setup Drive Motor pins, assuming motors 1 and 2 are
    are the right motors. Pin 2 and 3
    are connected to either side of the H-Bridge. A HIGH 
    value on Pin 2 and LOW value on Pin 3 will drive them
    both forward. Same setup for the left motors on pins 
    4 and 5.
  */
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  
  /*Setup Drive Motor speed. The pins that the H-Bridge enable
    pins are tied to need to be PWM pins. Using the analog
    write function of the Arduino we can vary the speed of the 
    motors from 0 to Full On using a PWM signal with a duty cycle
    determined by the value written to the pin. The value ranges 
    from 0 to 255. 0 meaning no output and 255 being full on. The 
    pins capable of doing this are 5 and 6.
  */
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  
  //Attach Servos and set initial angle
  leftFront.attach(7);
  leftRear.attach(8);
  rightFront.attach(9);
  rightRear.attach(10);
  platformRight.attach(26);
  platformLeft.attach(27);

  
  leftFront.write(90);
  leftRear.write(90);
  rightFront.write(90);
  rightRear.write(90);
  platformRight.write(90);
  platformLeft.write(90);

  //Setup Edge detectioin ISR
  attachInterrupt(3, sensorISR, RISING);
}

void loop(){
  oldLoopTime = millis();
  state = SLEEP;
  //Loop until we get 5 bytes from the UART then read them into inPacket
  StatusByte = 0;
  if(Serial.available() == 5){
    state = NORMAL;
    for(int i = 0; i < 5 ; i++){
      inPacket[i] = Serial.read();
    }
    
    StatusByte = 0x02 | StatusByte; 
    Serial.write(StatusByte);
    
  }
  
  //Normal packet received state
  if (state == NORMAL){
    //Figure out what motors are called for
    switch (inPacket[0]){
      
      //Drive motors called
    case DRIVE: 
      
      MotorArray[0] = inPacket[1];
      MotorArray[1] = inPacket[2];
      MotorArray[2] = inPacket[3];
      MotorArray[3] = inPacket[4];
      
      //Set Direction, Assuming we want both front and back going the same way
      if (MotorArray[2] == FORWARD){
	digitalWrite(2, HIGH);
	digitalWrite(3, LOW);
        
      }
      else {
	digitalWrite(2, LOW);
	digitalWrite(3, HIGH);
        
      }
      
      if (MotorArray[3] == FORWARD){
	digitalWrite(4, HIGH);
	digitalWrite(5, LOW);
      }
      
      else {
	digitalWrite(4, LOW);
	digitalWrite(5, HIGH);
      }
      
      
      //Next set speed
      analogWrite(11, MotorArray[0]);
      analogWrite(12, MotorArray[1]);
      
      break;
      
    case STEER:
      
      SteerArray[0] = inPacket[1];
      SteerArray[1] = inPacket[2];
      SteerArray[2] = inPacket[3];
      SteerArray[3] = inPacket[4];
      //Check and set servo angles
      if(SteerArray[0] <= 180){
	leftFront.write(SteerArray[0]);
      }
      else{
	StatusByte = 0x4 | StatusByte;
      }
      
      if(SteerArray[1] <= 180){
	leftRear.write(SteerArray[1]);
      }
      else{
	StatusByte = 0x4 | StatusByte;
      }
      
      if(SteerArray[2] <= 180){
	rightFront.write(SteerArray[2]);
      }
      else{
	StatusByte = 0x4 | StatusByte;
      }
      
      if(SteerArray[3] <= 180){
	rightRear.write(SteerArray[3]);
      }
      else{
	StatusByte = 0x4 | StatusByte;
      }        
      break;
      
      
    case SENSOR:
      switch (inPacket[1]){
      case 0:
	SensorArray[0] = analogRead(0);
	break;
	
      case 1:
	SensorArray[1] = analogRead(1);
	break;
	
      case 2:
	pinMode(22, OUTPUT);
	digitalWrite(22, LOW);
	delayMicroseconds(2);
	digitalWrite(22, HIGH);
	delayMicroseconds(5);
	digitalWrite(22, LOW);
	pinMode(22, INPUT);    
	SensorArray[2] = (pulseIn(22, HIGH, 10000));
	break;
        
	
      case 3:
	pinMode(23, OUTPUT);
	digitalWrite(23, LOW);
	delayMicroseconds(2);
	digitalWrite(23, HIGH);
	delayMicroseconds(5);
	digitalWrite(23, LOW);
	pinMode(23, INPUT);    
	SensorArray[3] = (pulseIn(23, HIGH, 10000));
	break;
        
      }
      
      Serial.write(SensorArray[inPacket[1]] & 0xFF);
      Serial.write((SensorArray[inPacket[1]] >> 8) & 0xFF);
      break;
      
    case STATUS:
      Serial.write(MotorArray, 4);
      Serial.write(SteerArray, 4);
      Serial.write(loopTime & 0xFF);
      Serial.write((loopTime >> 8) & 0xFF);
      
      break;
      
      //Platform packet received, drive platform up or down
    case PLATFORM:
      if(inPacket[1] == 'U'){
	//Move Platform Up
	platformRight.write(180);
	platformLeft.write(180);
      }
      if(inPacket[1] == 'D'){
	//Move Platform Down
	platformRight.write(0);
	platformLeft.write(0);
      }
      if(inPacket[1] == 'S'){
	platformRight.write(90);
	platformLeft.write(90);
      }
      break;
      
    default:
      break;
    }
  }
  
  //Error State
  if(state == ERROR){
    
    analogWrite(11, 0);
    analogWrite(12, 0);
    errorFlag = 1;
    //Hang here and still receive packets from computer
    while(errorFlag){
      if(Serial.available() == 5){
        state = NORMAL;
        for(int i = 0; i < 5 ; i++){
          inPacket[i] = Serial.read();
        }
        
        StatusByte = 0x01 | StatusByte; 
        Serial.write(StatusByte);
        
      }
      
      switch(inPacket[0]){
       case DRIVE: 
	
	MotorArray[0] = inPacket[1];
	MotorArray[1] = inPacket[2];
	MotorArray[2] = inPacket[3];
	MotorArray[3] = inPacket[4];
	
	//Set Direction, Assuming we want both front and back going the same way
	if (MotorArray[2] == FORWARD){
	  digitalWrite(2, HIGH);
	  digitalWrite(3, LOW);
	  
	}
	else {
	  digitalWrite(2, LOW);
	  digitalWrite(3, HIGH);
	  
	}
	
	if (MotorArray[3] == FORWARD){
	  digitalWrite(4, HIGH);
	  digitalWrite(5, LOW);
	}
	
	else {
	  digitalWrite(4, LOW);
	  digitalWrite(5, HIGH);
	}
	
	
	//Next set speed
	analogWrite(11, MotorArray[0]);
	analogWrite(12, MotorArray[1]);
        
	break;
	
      case STEER:
	
	SteerArray[0] = inPacket[1];
	SteerArray[1] = inPacket[2];
	SteerArray[2] = inPacket[3];
	SteerArray[3] = inPacket[4];
	//Check and set servo angles
	if(SteerArray[0] <= 180){
	  leftFront.write(SteerArray[0]);
	}
	else{
	  StatusByte = 0x4 | StatusByte;
	}
	
	if(SteerArray[1] <= 180){
	  leftRear.write(SteerArray[1]);
	}
	else{
	  StatusByte = 0x4 | StatusByte;
	}
	
	if(SteerArray[2] <= 180){
	  rightFront.write(SteerArray[2]);
	}
	else{
	  StatusByte = 0x4 | StatusByte;
	}
	
	if(SteerArray[3] <= 180){
	  rightRear.write(SteerArray[3]);
	}
	else{
	  StatusByte = 0x4 | StatusByte;
	}        
	break;

      case BREAKOUT:
	errorFlag = 0;
        rightRear.write(180);
        delayMicroseconds(100000000);
        state = NORMAL;
	analogWrite(11, 0);
	analogWrite(12, 0);
        break;
      }
    }
  }
	
	
  loopTime = millis() - oldLoopTime;  
  LoopCount++;
}
      
      
      
      
