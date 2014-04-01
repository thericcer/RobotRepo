#include "controller.hpp"
#include <iostream>
#include <stdio.h>
#include <syslog.h>

//#define DEBUG


Controller::Controller(std::string file):connected(-1), trim1(0),trim2(0),trim3(0),trim4(0), boomLower(77), boomLowerHome(77), boomUpper(70), boomUpperHome(70), hookHome(170){

  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);

  connected = serialPort.open_port(file);

  if(connected > 0){
    #ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Port Sucessfully Opened!");
    #endif
  }
  else{
    #ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR), "Could Not Open Port!");
    #endif
  }

  closelog();
  sleep(2);
}
  
int Controller::drive(unsigned char s1, unsigned char s2, char dir1, char dir2){


  unsigned char packet[5] = {'D', s1, s2, dir1, dir2};

  while(!mtx.try_lock());

  if(connected){

#ifdef DEBUG
    openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Sent drive packet");
#endif

    //Write packet and wait for status byte
    serialPort.m_write(packet, 5);
    while(serialPort.peek() < 1);
    serialPort.m_read(&status, 1);
  }

  mtx.unlock();

  //Return based on status
  if(status == 0x2){
    return 1;
  }
  else{
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }

#ifdef DEBUG
  closelog();
#endif
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
  if(connected){

#ifdef DEBUG
    openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Sending Steering Packet: %d, %d, %d, %d", a1, a2, a3, a4);
#endif

    //Write packet and wait for status byte
    serialPort.m_write(packet, 5);
    while(serialPort.peek() < 1);
    serialPort.m_read(&status, 1);
  }
  mtx.unlock();

  if (status == 0x2){
    return 1;
  }
  else{
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }

#ifdef DEBUG
  closelog();
#endif
}

int Controller::getSensor(unsigned char sensor, unsigned short* sensorValue){
  unsigned char packet[5] = {'Z', sensor, 'x', 'x', 'x'};
  unsigned char inputBuffer[2] = {0};

  while(!mtx.try_lock());

#ifdef DEBUG
  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Qwerying for sensor %d", sensor);
#endif

  if(connected){
    //Write packet and wait for first byte, status byte.
    serialPort.m_write(packet, 5);
    while(serialPort.peek() < 1);
    serialPort.m_read(&status, 1);
    
    //Wait for two more bytes and throw those into the sensorValue short.
    while(serialPort.peek() < 2);
    serialPort.m_read(inputBuffer, 2);

#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Got %X and %X from the micro", inputBuffer[0], inputBuffer[1]);
#endif

    //Combine valued into short
    *sensorValue = (inputBuffer[0]&0xFF | inputBuffer[1]<<8);
    //    printf("LOW: %X\t HIGH: %X\tOUTPUT: %X\n", (inputBuffer[0]), inputBuffer[1], *sensorValue);

#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Combined into %d", *sensorValue);
#endif
    
  }
  
  mtx.unlock();

  //Return based on status byte
  if (status == 0x2){
    return 1;
  }
  else{
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }
#ifdef DEBUG
  closelog();
#endif
}

int Controller::movePlatform(char direction){
  unsigned char packet[5] = {'P', direction, 'x', 'x', 'x'};
  
  while(!mtx.try_lock());

  if(connected){
#ifdef DEBUG
  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Moving Platform %c", direction);
#endif
    //Write packet and wait for status byte
    serialPort.m_write(packet, 5);
    while(serialPort.peek() < 1);
    serialPort.m_read(&status, 1);
  }
  mtx.unlock();

  if (status == 0x2){
    return 1;
  }
  else {
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }
#ifdef DEBUG
  closelog();
#endif
}

int Controller::platformPosition(unsigned short* position){
  unsigned char packet[5] = {'Y', 0, 0, 0, 0};
  unsigned char tempPosition[2] = {0};

  while(!mtx.try_lock());

#ifdef DEBUG
  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Getting Platform Position");
#endif

  serialPort.m_write(packet, 5);
  while(serialPort.peek() < 1);
  serialPort.m_read(&status, 1);

#ifdef DEBUG
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Waiting for position");
#endif

  while(serialPort.peek() < 2);
  serialPort.m_read(tempPosition, 2);

  *position = (tempPosition[0] & 0xFF) | (tempPosition[1]<<8);

  mtx.unlock();

  if (status == 0x2){
    return 1;
  }
  else{
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }
#ifdef DEBUG
  closelog();
#endif
}


int Controller::deployHook(unsigned char position){
  unsigned char packet[5] = {'H', position, 'x', 'x', 'x'};
  int i;
  while(!mtx.try_lock());

  if(connected){

#ifdef DEBUG
  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Deploying Hook to %d", position);
#endif

    //write packet on serial port
    serialPort.m_write(packet, 5);
    while(serialPort.peek()<1);
    serialPort.m_read(&status, 1);    
  }

  mtx.unlock();
  
  if (status == 0x2){
    return 1;
  }
  else{
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }
#ifdef DEBUG
  closelog();
#endif
}

int Controller::retractHook(void){
  unsigned char packet[5] = {'H', hookHome, 0, 0, 0};
  int i;

  while(mtx.try_lock());

  if(connected){

#ifdef DEBUG
  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Returning Hook to %d", hookHome);
#endif

    serialPort.m_write(packet, 5);
    while(serialPort.peek()<1);
    serialPort.m_read(&status, 1);
  }

  mtx.unlock();

  if(status == 0x2){
    return 1;
  }
  else{
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }
#ifdef DEBUG
  closelog();
#endif
}

int Controller::deployCamera(unsigned char lower, unsigned char upper){
  unsigned char packet[5] = {'B', boomLowerHome, boomUpperHome, 0, 0};
  int i;

  while(!mtx.try_lock()); 

  if(connected){

#ifdef DEBUG
  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Deploying camera boom to L:%d   U:%d", lower, upper);
#endif

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
  }
  mtx.unlock();
  
  if(status == 0x2){
    return 1;
  }
  else{
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }
#ifdef DEBUG
  closelog();
#endif
}
    
int Controller::retractCamera(void){
  unsigned char packet[5] = {'B', boomLowerHome, boomUpperHome, 0, 0};
  int i;
  
  while(!mtx.try_lock());
  
  if(connected){

#ifdef DEBUG
  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Returning camera boom to L:%d  U:%d", boomLowerHome, boomUpperHome);
#endif
    
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
  }
  mtx.unlock();
  
  if(status == 0x2){
    return 1;
  }
  else{
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }
#ifdef DEBUG
  closelog();
#endif
}

int Controller::pusher(char direction){
  unsigned char packet[5]={'K','S',0,0,0};
  unsigned char done;
  
  while(!mtx.try_lock());

  if(connected){
    
#ifdef DEBUG
  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Moving Pusher %c",direction);
#endif

    if (direction=='F'){
      
      packet[1]='F';
      serialPort.m_write(packet,5);
      while(serialPort.peek()<1);
      serialPort.m_read(&status,1);
      
      //Wait until it's done
    }
    if (direction=='S'){
      
      packet[1]='S';
      serialPort.m_write(packet,5);
      while(serialPort.peek()<1);
      serialPort.m_read(&status,1);

    }
    if (direction=='R'){
      
      packet[1]='R';
      serialPort.m_write(packet,5);
      while(serialPort.peek()<1);
      serialPort.m_read(&status,1);

    }
    
    //Wait until it's done
    while(serialPort.peek()<1);
    serialPort.m_read(&done, 1);
#ifdef DEBUG
    if(done == 'D'){
      syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Got valid finish from pusher");
    }
    else{
      syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Pusher returned invalid finish!");
    }
#endif
  mtx.unlock(); 
  if(status == 0x2){
    return 1;
  }
 else{
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }
#ifdef DEBUG
  closelog();
#endif
  }
}
    
    
int Controller::getStatus(unsigned char* statusArray){
  unsigned char packet[5] = {'C', 'x', 'x', 'x', 'x'};
  
  while(!mtx.try_lock());
  
  if(connected){

#ifdef DEBUG
  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Getting Micro Status");
#endif
    //Write Packet and wait for status
    serialPort.m_write(packet, 5);
    while(serialPort.peek() < 1);
    serialPort.m_read(&status, 1);
    
    //Wait until there are 10 bytes in the input buffer, then read them all in
    while(serialPort.peek() < 10);
    serialPort.m_read(statusArray, 10);
  }

  mtx.unlock();

  //Return based on status
  if (status == 0x2){
    return 1;
  }
  else{
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }
#ifdef DEBUG
  closelog();
#endif
}

int Controller::voltage(float* volts){
  //Send voltage request
  unsigned char packet[5] = {'V', 0, 0, 0, 0};
  unsigned char data[2] = {0};
  short temp = 0;

  while(mtx.try_lock());
  
  if(connected){

#ifdef DEBUG
  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Reading main battery voltage");
#endif

  if(connected){
    serialPort.m_write(packet, 5);
    while(serialPort.peek()<1);
    serialPort.m_read(&status, 1);
    
    
    //We must read the 2 returned bytes.
    while(serialPort.peek()<2);
    serialPort.m_read(data, 2);
    
    temp = data[0] | data[1]<<8;
       
    *volts = (5.0/1024) * temp * 3;

#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Got %.2fv", *volts);
#endif
      }
  mtx.unlock();

  if (status == 0x2){
    return 1;
  }
  else{
#ifdef DEBUG
    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Got bad status %X", status);
#endif
    return -1;
  }
#ifdef DEBUG
  closelog();
#endif
  }
}
  
int Controller::close(void){
  if(connected){

#ifdef DEBUG
  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Closing down robot");
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Stopping Drive Motors");
#endif

    drive(0, 0, 'F', 'F');
    
#ifdef DEBUG
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Setting Steering Servos to 90");
#endif

    steer(90, 90, 90, 90);

#ifdef DEBUG
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Retracting Camera");
#endif
   
    retractCamera();
   
#ifdef DEBUG
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Retracting Hook");
#endif

    retractHook();
    
#ifdef DEBUG
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Positioning Pusher");
#endif
    
    pusher('F');

#ifdef DEBUG
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Closing Serial Port");
  closelog();
#endif
    serialPort.close_port();
  }
}
