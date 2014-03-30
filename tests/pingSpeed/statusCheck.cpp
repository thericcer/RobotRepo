#include <controller.cpp>
#include <stdio.h>

int main(void){

  Controller* controller = new Controller("/dev/ttyACM0");
  unsigned short sensors[2] = {0};
  char status[2] = {0};
  while(1){
    status[0] = controller->getSensor(2, &sensors[0]);
    status[1] = controller->getSensor(3, &sensors[1]);
    printf("Sensor1:\t%d:%d\tSensor2:\t%d:%d\n", status[0], sensors[0], status[1], sensors[1]);
    if(status[0] != 1 || status[1] != 1){
      printf("FUCKED!\n");
      sleep(1);
    }
    if(sensors[0] > 10000 || sensors[1] > 10000){
      printf("BAD VALUE\n");
      sleep(1);
    }
  }
}
