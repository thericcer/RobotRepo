#include "controller.hpp"
#include <iostream>
#include <string.h>
#include <stdio.h>

int main (int argc, char** argv){
  Controller controller("/dev/ttyACM0");
  unsigned short sensor[2] = {666};



  printf("Drive packet | Got %d\n\a",  controller.drive(255, 255, 'F', 'F'));
  printf("Steer Packet | Got %d\n\a", controller.steer(180, 180, 180, 180));

  for (int i=0; i < 10; i++){
    controller.getSensor(2, sensor);
    controller.getSensor(3, &sensor[1]);
    printf("%10d | %10d\n\a", sensor[0], sensor[1]);
    sleep(1);
  }
  return 0;
}

