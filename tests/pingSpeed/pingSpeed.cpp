#include <controller.hpp>
#include <stdio.h>


Controller controller("/dev/ttyACM0");

int main(void){

  unsigned short sensors[2] = {0};


  while(1){
    controller.getSensor(2, &sensors[0]);
    controller.getSensor(3, &sensors[1]);

    printf("Sensor 1: %8d\tSensor 2: %8d\n", sensors[0], sensors[1]);
  }
  return 0;
}
