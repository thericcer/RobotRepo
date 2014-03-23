#include <controller.cpp>
#include <stdio.h>

Controller controller("/dev/ttyACM0");


int main(void){
  controller.deployCamera(35, 75);

  printf("Sleeping, then closing controller\n\n");
  sleep(5);

  controller.close();
  return 0;
}
