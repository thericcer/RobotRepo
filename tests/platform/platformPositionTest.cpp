#include <controller.cpp>
#include <stdio.h>

Controller* controller = new Controller("/dev/ttyACM0");

unsigned short correctHeight(float inches){
  float ans = 0;
  unsigned short sensors = 0;

  ans = ((inches-2.4514)/0.928)*100;
  sensors = ans;
  return sensors;
}

int main(void){

  unsigned short position = 0;

  controller->deployHook(0);
  controller->pusher('R');

  printf("Going to go up to %d\n", correctHeight(2.88)+5);
  getchar();

  while(position < (correctHeight(2.88)+5)){
    controller->platformPosition(&position);
    printf("Position: %d\n", position);
    controller->movePlatform('U');
  }
  controller->movePlatform('S');
  controller->pusher('F');
  controller->pusher('R');
  sleep(5);
  while(position > 0){
    controller->platformPosition(&position);
    printf("Position: %d\n", position);
    controller->movePlatform('D');
  }
  controller->movePlatform('D');
  sleep(5);
  controller->movePlatform('S');
  return 0;
}
