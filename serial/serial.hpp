#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<termios.h> 
#include<iostream>
#include<sstream>
#include<sys/ioctl.h> 

class Serial
{
  
private:
  int fd; // file descriptor for serial interface
  struct termios options; // a data structure for serial config flags
  char buffer[64]; // an output buffer
  int wr, rd;  // return values for confirmation of read and write commands
  
public:
  int open_port(std::string n_string);
  int m_write(unsigned char* _char, int _length);
  int m_read(char* r_string, int bytes);
  int peek();
  void clear();
  int close_port();

};
