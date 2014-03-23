#include <controller.cpp>
#include <stdio.h>

Controller controller("/dev/ttyACM0");


int main(void){
  controller.deployCamera(150, 150);

  sleep(1);

  controller.close();
  return 0;
}
