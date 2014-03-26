#include <controller.hpp>
#include <stdio.h>

Controller controller("/dev/ttyACM0");


int main(void){

  printf("Deploying Robot things!\n");

  sleep(1);
  printf("Hook\n");
  controller.deployHook(0);
  sleep(1);
  printf("Camera Boom\n");
  controller.deployCamera(50, 45);
  sleep(1);
  printf("Pusher R\n");
  controller.pusher('R');
  sleep(15);
  printf("Pusher S\n");
  controller.pusher('S');

  printf("Platform\n");
  controller.movePlatform('U');
  sleep(15);
  controller.movePlatform('D');
  sleep(10);
  controller.movePlatform('S');
  


  printf("Deployed!\n");

  controller.close();
}
