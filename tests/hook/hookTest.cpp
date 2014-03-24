#include<controller.hpp>
#include<stdio.h>

int main(void){


  Controller controller("/dev/ttyACM0");

  controller.deployHook(57);
  sleep(5);
  controller.deployHook(120);
  sleep(2);
  controller.retractHook();

  return 1;
}
