#include <controller.cpp>
#include <stdio.h>

Controller controller("/dev/ttyACM0");


int main(void){

  printf("Calling Pusher\n");

  controller.pusher('F');

  printf("Returned from pusher!\n");

  printf("Calling Reverse\n");
  controller.pusher('R');
  printf("Returned FFIDLSHDK\n");

  return 0;
}
