#include <controller.cpp>
#include <stdio.h>

Controller* controller = new Controller("/dev/ttyACM0");

int main(void){

  unsigned short position = 0;
  
  while(position < 322){
    controller->platformPosition(&position);
    printf("Position: %d\n", position);
    controller->movePlatform('U');
  }
  controller->movePlatform('S');
  getchar();
  while(position < 457){
    controller->platformPosition(&position);
    printf("Position: %d\n", position);
    controller->movePlatform('U');
  }
  controller->movePlatform('S');
  getchar();
  while(position < 893){
    controller->platformPosition(&position);
    printf("Position: %d\n", position);
    controller->movePlatform('U');
  }
  controller->movePlatform('S');
  getchar();

  while(position > 0){
    controller->platformPosition(&position);
    printf("Position: %d\n", position);
    controller->movePlatform('D');
  }
  controller->movePlatform('S');
  return 0;
}
