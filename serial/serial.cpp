#include"serial.hpp"
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<termios.h>
#include<iostream>
#include<sstream>
#include<sys/ioctl.h>

int Serial::open_port(std::string n_string)
{
  fd = open(n_string.c_str(), O_RDWR | O_NOCTTY | O_NDELAY); //
  // get options for the port
  tcgetattr(fd, &options);
  
  // set the baudrates to 9600
  cfsetispeed(&options, B9600);
  cfsetospeed(&options, B9600);
  
  // enable receiver and set local mode
  options.c_cflag |= (CLOCAL | CREAD);
  // setup the character size
  options.c_cflag &= ~CSIZE; // mask the character size bits
  options.c_cflag |= CS8; // select 8 data bits
  // no parity 
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  // disable hardware flow control
  options.c_cflag &= ~CRTSCTS;
  // ( canonical)
  //options.c_lflag |= (ICANON | ECHO | ECHOE );
  
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  
  // ignore parity
  //	options.c_iflag |= IGNPAR;
  
  // kill software flow control
  options.c_iflag &= ~(IXON | IXOFF | IXANY);
  // raw output
  options.c_oflag &= ~OPOST;
  
  options.c_cc[VTIME] = 5;
  
  //set the new options for the port
  tcsetattr(fd, TCSANOW, &options);
  
  
  if (fd == -1){
    std::cout << "this bombed\n";
    return -1;
  }
  else
    {
      fcntl(fd, F_SETFL,FNDELAY); 
      std::cout << "it worked and the fd = " << fd << "\n";
      return 1;
    }
}

int Serial::m_write(unsigned char* _char, int _length) {
  return write(fd, _char, _length);
}

int Serial::m_read(char* o_string, int bytes)
{
  int bytes_available;
  ioctl(fd, FIONREAD, &bytes_available);
  //std::cout << bytes_available << " bytes available to read\n";
  
  if(bytes > bytes_available){
    std::cout << "Reading more than available!\n";
    return -1;
  }
  
  rd = read(fd, o_string, bytes);
  //o_string = buffer;
  //tcdrain(fd);
  return rd;
}

int Serial::peek()
{
  int bytes_available;
  ioctl(fd, FIONREAD, &bytes_available);
  return bytes_available;
}

void Serial::clear()
{
  tcflush(fd, TCIOFLUSH);
  //	std::cout << "clear got called\n";
}

int Serial::close_port(){
  close(fd);
}
