#include <serial.hpp>
#include <iostream>
#include <string.h>

class Controller{

private:
  Serial serialPort; // serial class object
  char status; //Current status byte

  //Trim values for each of the steering motors
  char trim1;
  char trim2;
  char trim3;
  char trim4;
  

public:

  /*Constructor opens serial port and serial port location "/dev/ttyXX" is passed in*/
  Controller(std::string file);

  /*This function takes four values for trimming the steering servos. */
  void trim(unsigned char t1, unsigned char t2, unsigned char t3, unsigned char t4);

  /*Drive takes 4 arguments. The first 2 are speeds, 0-255. The Last 2 are the directions, F or R.
    The function return 1 upon receiving an 0x2 from the controller and it returns -1 if something
    else is returned. THE FUNCTION WILL HANG if nothing is returned from the controller.
  */
  int drive(unsigned char s1, unsigned char s2, char dir1, char dir2);

  /*Steer takes 4 arguments. Each one is an angle from 0-180.
    The function returns 1 upon receiving an 0x2 from the controller and it returns -1 if something
    else is returned. THIS FUNCTION WILL HANG if nothging is returned from the controller.
  */
  int steer(char a1, char a2, char a3, char a4);

  /*Get Sensor takes 2 arguments. The sensor number requested and the address of an unsigned short
    to put the sensor value into. The function will return a 1 upon receiving an 0x2 from the controller
    and a -1 if something else is returned. After the function is finished the sensor value will be
    in the memory location passed in for sensorValue
  */
  int getSensor(unsigned char sensor, unsigned short* sensorValue);

  /*This function controlls the platform. a U moves the platform up, a D moves the platform down
    and an S stops the platform
  */
  int movePlatform(char direction);

  /*Takes an angle between 0 and 180 and it moves the position of the hook to that angle*/
  int moveHook(unsigned char angle);

  /*Set Status takes one argument, an address to the start of a 10 character array. When called it 
    will pull all stored datastructures from the controller to verify data sent to the controller.
    This function waits for all data from the controller to be in the serial buffer until attemting
    to read it out. This function will take the longest time. 
    Data is in the form of:
    char MotorArray = s1, s2, d1, d2
    char SteeringArray = a1, a2, a3, a4
    unsigned short LoopTime in ms
  */
  int getStatus(char* statusArray);

};  
