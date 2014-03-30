#include <controller.hpp>
#include <stdio.h>
#include <iostream>


Controller controller("/dev/ttyACM0");


int main(void){

  printf("Deploying Robot things!\n");

  controller.steer(105, 105, 105, 105);
  sleep(1);
  controller.steer(90, 90, 90, 90);
  sleep(1);
  controller.drive(255, 255, 'F', 'F');
  usleep(500000);
  controller.drive(255, 255, 'R', 'R');
  usleep(500000);
  controller.drive(0, 0, 'F', 'F');

  sleep(1);
  printf("Hook\n");
  controller.deployHook(90);
  sleep(1);
  printf("Camera Boom\n");
  controller.deployCamera(3, 15);
  sleep(1);
  printf("Pusher R\n");
  controller.pusher('R');
  printf("Pusher S\n");
  controller.pusher('S');


  printf("Platform\n");
  controller.movePlatform('U');
  sleep(5);
  controller.movePlatform('D');
  sleep(4);
  controller.movePlatform('S');

 
 
  printf("Deployed!\n");

  getchar();

  controller.close();
}
