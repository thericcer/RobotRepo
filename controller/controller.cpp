#include "controller.hpp"
#include <iostream>

Controller::Controller(std::string file):trim1(0),trim2(0),trim3(0),trim4(0){
  serialPort.open_port(file);
  sleep(2);
}
  

int Controller::drive(unsigned char s1, unsigned char s2, char dir1, char dir2){
  unsigned char packet[5] = {'D', s1, s2, dir1, dir2};

  while(!mtx.try_lock());

  //Write packet and wait for status byte
  serialPort.m_write(packet, 5);
  while(serialPort.peek() < 1);
  serialPort.m_read(&status, 1);

  mtx.unlock();

  //Return based on status
  if(status == 0x2){
    return 1;
  }
  else{
    return -1;
  }
}

void Controller::trim(unsigned char t1, unsigned char t2, unsigned char t3, unsigned char t4){
  trim1 = t1;
  trim2 = t2;
  trim3 = t3;
  trim4 = t4;
}

int Controller::steer(char a1, char a2, char a3, char a4){

  a1 = a1 + trim1;
  a2 = a2 + trim2;
  a3 = a3 + trim3;
  a4 = a4 + trim4;

  if(a1 > 180){
    a1 = 180;
  }
  if(a2 > 180){
    a2 = 180;
  }
  if(a3 > 180){
    a3 = 180;
  }
  if(a4 > 180){
    a4 = 180;
  }

  unsigned char packet[5] = {'S', a1, a2, a3, a4};

  while(!mtx.try_lock());

  //Write packet and wait for status byte
  serialPort.m_write(packet, 5);
  while(serialPort.peek() < 1);
  serialPort.m_read(&status, 1);

  mtx.unlock();

  if (status == 0x2){
    return 1;
  }
  else{
    return -1;
  }
}

int Controller::getSensor(unsigned char sensor, unsigned short* sensorValue){
  unsigned char packet[5] = {'Z', sensor, 'x', 'x', 'x'};
  char inputBuffer[2] = {0};

  while(!mtx.try_lock());

  //Write packet and wait for first byte, status byte.
  serialPort.m_write(packet, 5);
  while(serialPort.peek() < 1);
  serialPort.m_read(&status, 1);

  //Wait for two more bytes and throw those into the sensorValue short.
  while(serialPort.peek() < 2);
  serialPort.m_read(inputBuffer, 2);

  //Combine valued into short
  *sensorValue = (inputBuffer[1] << 8) + inputBuffer[0];

  mtx.unlock();

  //Return based on status byte
  if (status == 0x2){
    return 1;
  }
  else{
    return -1;
  }
}

int Controller::movePlatform(char direction){
  unsigned char packet[5] = {'P', direction, 'x', 'x', 'x'};
  
  while(!mtx.try_lock());

  //Write packet and wait for status byte
  serialPort.m_write(packet, 5);
  while(serialPort.peek() < 1);
  serialPort.m_read(&status, 1);

  mtx.unlock();

  if (status == 0x2){
    return 1;
  }
  else {
    return -1;
  }
}

int Controller::moveHook(unsigned char angle){
  unsigned char packet[5] = {'H', angle, 'x', 'x', 'x'};

  while(!mtx.try_lock());

  //write packet on serial port
  serialPort.m_write(packet, 5);
  while(serialPort.peek()<1);
  serialPort.m_read(&status, 1);

  mtx.unlock();

  if (status == 0x2){
    return 1;
  }
  else{
    return -1;
  }
}

int Controller::getStatus(char* statusArray){
  unsigned char packet[5] = {'C', 'x', 'x', 'x', 'x'};

  while(!mtx.try_lock());

  //Write Packet and wait for status
  serialPort.m_write(packet, 5);
  while(serialPort.peek() < 1);
  serialPort.m_read(&status, 1);

  //Wait until there are 10 bytes in the input buffer, then read them all in
  while(serialPort.peek() < 10);
  serialPort.m_read(statusArray, 10);

  mtx.unlock();

  //Return based on status
  if (status == 0x2){
    return 1;
  }
  else{
    return -1;
  }
}
  
