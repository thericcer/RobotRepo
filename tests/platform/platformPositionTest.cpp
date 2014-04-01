#include <controller.cpp>
#include <stdio.h>

Controller* controller = new Controller("/dev/ttyACM0");

int main(void){

  unsigned short position = 0;

  /*  while(position < 400){
    controller->platformPosition(&position);
    printf("Position: %d\n", position);
    controller->movePlatform('U');
  }
  controller->movePlatform('S');
  sleep(5);
  while(position > 0){
    controller->platformPosition(&position);
    printf("Position: %d\n", position);
    controller->movePlatform('D');
    }*/
  controller->movePlatform('D');
  sleep(10);
  controller->movePlatform('S');
  return 0;
}
