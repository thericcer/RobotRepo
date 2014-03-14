#include<controller.hpp>
#include<stdio.h>

int main(void){


  Controller controller("/dev/ttyACM0");

  controller.moveHook(57);
  sleep(5);
  controller.moveHook(120);
  sleep(2);

  return 1;
}
