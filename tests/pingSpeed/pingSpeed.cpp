#include <controller.hpp>
#include <stdio.h>
#include <syslog.h>

Controller controller("/dev/ttyACM0");

int main(void){

  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);

  unsigned short sensors[6] = {0};


  while(1){
    controller.getSensor(0, &sensors[0]);
    controller.getSensor(1, &sensors[1]);
    controller.getSensor(2, &sensors[2]);
    controller.getSensor(3, &sensors[3]);
    //    controller.getSensor(4, &sensors[4]);
    //    controller.getSensor(5, &sensors[5]);

    syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "Sensor Readings: 0: %5d\t1: %5d\t2: %5d\t3: %5d\t4: %5d\t5: %5d", sensors[0], sensors[1], sensors[2], sensors[3], sensors[4], sensors[5]);
  }
  return 0;
}
