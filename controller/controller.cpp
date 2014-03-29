#include "controller.hpp"
#include <iostream>
#include <stdio.h>
#include <syslog.h>

Controller::Controller(std::string file):trim1(0),trim2(0),trim3(0),trim4(0), boomLower(77), boomLowerHome(77), boomUpper(70), boomUpperHome(70), hookHome(170){

  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);

  if(serialPort.open_port(file) > 0){
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Port Sucessfully Opened!");
  }
  else{
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR), "Could Not Open Port!");
  }

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

void Controller::trim(char t1, char t2, char t3, char t4){
  trim1 = t1;
  trim2 = t2;
  trim3 = t3;
  trim4 = t4;
}

int Controller::steer(unsigned char a1, unsigned char a2, unsigned char a3, unsigned char a4){



  if(trim1 < 0){
    if (a1 < fabs(trim1)){
      a1 = 0;
    }
    else{
      a1 = a1 + trim1;
    }      
  }
  else{
    if(a1 + trim1 > 180){
      a1 = 180;
    }
  }

  if(trim2 < 0){
    if (a2 < fabs(trim2)){
      a2 = 0;
    }
    else{
      a2 = a2 + trim2;
    }      
  }
  else{
    if(a2 + trim2 > 180){
      a2 = 180;
    }
  }

  if(trim3 < 0){
    if (a3 < fabs(trim3)){
      a3 = 0;
    }
    else{
      a3 = a3 + trim3;
    }      
  }
  else{
    if(a3 + trim3 > 180){
      a3 = 180;
    }
  }

  if(trim4 < 0){
    if (a4 < fabs(trim4)){
      a4 = 0;
    }
    else{
      a4 = a4 + trim4;
    }      
  }
  else{
    if(a4 + trim4 > 180){
      a4 = 180;
    }
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

int Controller::deployHook(unsigned char position){
  unsigned char packet[5] = {'H', position, 'x', 'x', 'x'};
  int i;
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

int Controller::retractHook(void){
  unsigned char packet[5] = {'H', hookHome, 0, 0, 0};
  int i;

  while(mtx.try_lock());

  serialPort.m_write(packet, 5);
  while(serialPort.peek()<1);
  serialPort.m_read(&status, 1);

  mtx.unlock();

  if(status == 0x2){
    return 1;
  }
  else{
    return -1;
  }
}

int Controller::deployCamera(unsigned char lower, unsigned char upper){
  unsigned char packet[5] = {'B', boomLowerHome, boomUpperHome, 0, 0};
  int i;

  while(!mtx.try_lock()); 

  if(boomLowerHome < 180 && boomUpperHome < 180){
    serialPort.m_write(packet, 5);
    while(serialPort.peek() < 1);
    serialPort.m_read(&status, 1);
  }
  else{
    packet[0] = 'B';
    packet[1] = 180;
    packet[2] = 180;
    packet[3] = 0;
    packet[4] = 0;
    serialPort.m_write(packet, 5);
    while(serialPort.peek() < 1);
    serialPort.m_read(&status, 1);
  }
  if(boomLowerHome < lower && boomUpperHome < upper){
    for(i=boomLowerHome; i < lower; i+=1){
          packet[0] = 'C';
	  packet[1] = i;
	  packet[2] = boomUpperHome;
	  packet[3] = 0;
	  packet[4] = 0;
	  serialPort.m_write(packet, 5);
	  while(serialPort.peek() < 1);
	  serialPort.m_read(&status, 1);
	  usleep(10000);
	  boomLower = i;
	  boomUpper = boomUpperHome;

    }
    
    for(i=boomUpperHome; i < upper; i+=1){
          packet[0] = 'C';
	  packet[1] = lower;
	  packet[2] = i;
	  packet[3] = 0;
	  packet[4] = 0;
     
	  serialPort.m_write(packet, 5);
	  while(serialPort.peek() < 1);
	  serialPort.m_read(&status, 1);
	  usleep(10000);
	  boomUpper = i;
    }
  }

  else{
    for(i=boomLowerHome; i > lower; i-=1){
          packet[0] = 'C';
	  packet[1] = i;
	  packet[2] = boomUpperHome;
	  packet[3] = 0;
	  packet[4] = 0;
	  serialPort.m_write(packet, 5);
	  while(serialPort.peek() < 1);
	  serialPort.m_read(&status, 1);
	  usleep(10000);
	  boomLower = i;
	  boomUpper = boomUpperHome;
    }
    
    for(i=boomUpperHome; i > upper; i-=1){
          packet[0] = 'C';
	  packet[1] = lower;
	  packet[2] = i;
	  packet[3] = 0;
	  packet[4] = 0;
     
	  serialPort.m_write(packet, 5);
	  while(serialPort.peek() < 1);
	  serialPort.m_read(&status, 1);
	  usleep(10000);
	  boomUpper = i;
    }
  }
    
  mtx.unlock();
  
  if(status == 0x2){
    return 1;
  }
  else{
    return -1;
  }
}

int Controller::retractCamera(void){
  unsigned char packet[5] = {'B', boomLowerHome, boomUpperHome, 0, 0};
  int i;

  while(!mtx.try_lock());

  if(boomLower < boomLowerHome && boomUpper < boomUpperHome){
    for(i=boomLower; i < boomLowerHome; i+=1){
          packet[0] = 'C';
	  packet[1] = i;
	  packet[2] = boomUpper;
	  packet[3] = 0;
	  packet[4] = 0;
	  serialPort.m_write(packet, 5);
	  while(serialPort.peek() < 1);
	  serialPort.m_read(&status, 1);
	  usleep(10000);
    }
    
    for(i=boomUpper; i < boomUpperHome; i+=1){
          packet[0] = 'C';
	  packet[1] = boomLowerHome;
	  packet[2] = i;
	  packet[3] = 0;
	  packet[4] = 0;
     
	  serialPort.m_write(packet, 5);
	  while(serialPort.peek() < 1);
	  serialPort.m_read(&status, 1);
	  usleep(10000);
    }
  }

  else{
    for(i=boomLower; i > boomLowerHome; i-=1){
          packet[0] = 'C';
	  packet[1] = i;
	  packet[2] = boomUpper;
	  packet[3] = 0;
	  packet[4] = 0;
	  serialPort.m_write(packet, 5);
	  while(serialPort.peek() < 1);
	  serialPort.m_read(&status, 1);
	  usleep(10000);
    }
    
    for(i=boomUpper; i > boomUpperHome; i-=1){
          packet[0] = 'C';
	  packet[1] = boomLowerHome;
	  packet[2] = i;
	  packet[3] = 0;
	  packet[4] = 0;
     
	  serialPort.m_write(packet, 5);
	  while(serialPort.peek() < 1);
	  serialPort.m_read(&status, 1);
	  usleep(10000);
    }
  }
    
  
  mtx.unlock();

  if(status == 0x2){
    return 1;
  }
  else{
    return -1;
  }
}

int Controller::pusher(char direction){
  unsigned char packet[5]={'K','S',0,0,0};
  char done;

  while(!mtx.try_lock());

  //  printf("Controller::pusher\n");
 
 if (direction=='F'){
   //   printf("Controller::pusher F\n");
   packet[1]='F';
   serialPort.m_write(packet,5);
   while(serialPort.peek()<1);
   serialPort.m_read(&status,1);

   //Wait until it's done
   while(serialPort.peek()<1);
   serialPort.m_read(&done, 1);
 }
 if (direction=='S'){
   //   printf("Controller::pusher S\n");
   packet[1]='S';
   serialPort.m_write(packet,5);
   while(serialPort.peek()<1);
   serialPort.m_read(&status,1);
 }
 if (direction=='R'){
   //   printf("Controller::pusher R\n");
   packet[1]='R';
   serialPort.m_write(packet,5);
   while(serialPort.peek()<1);
   serialPort.m_read(&status,1);

   //Wait until it's done
   while(serialPort.peek()<1);
   serialPort.m_read(&done, 1);
 }

 mtx.unlock(); 
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

int Controller::voltage(float* volts){
  //Send voltage request
  unsigned char packet[5] = {'V', 0, 0, 0, 0};
  char data[2] = {0};
  short temp = 0;

  while(mtx.try_lock());
  

  serialPort.m_write(packet, 5);
  while(serialPort.peek()<1);
  serialPort.m_read(&status, 1);


  //We must read the 2 returned bytes.
  while(serialPort.peek()<2);
  serialPort.m_read(data, 2);

  temp = data[0] | data[1]<<8;


  *volts = (5.0/1024) * temp * 3;

  mtx.unlock();

  if (status == 0x2){
    return 1;
  }
  else{
    return -1;
  }
}
  
int Controller::close(void){
  
  drive(0, 0, 'F', 'F');

  steer(90, 90, 90, 90);

  retractCamera();

  retractHook();


  pusher('R');
  pusher('F');
  pusher('S');
  serialPort.close_port();
}
