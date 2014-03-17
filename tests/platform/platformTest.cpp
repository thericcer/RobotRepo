#include <controller.hpp>
#include <stdio.h>

Controller controller("/dev/ttyACM0");


int main(void){

  controller.movePlatform('U');
  sleep(1);
  controller.movePlatform('S');
  sleep(1);
  controller.movePlatform('D');
  sleep(1);
  controller.movePlatform('S');
  return 0;
}
