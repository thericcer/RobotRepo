#include <controller.hpp>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>

Controller controller("/dev/ttyACM0");
unsigned short sensors[4] = {0};
double angle = 0;


/* This is a function to kill the robot upon catching a keyboard
   or other interrupt
*/
void killed(int sig){
  printf("\n\nProgram Killed!\n");
  controller.drive(0, 0, 'F', 'F');
  controller.steer(90, 90, 90, 90);
  printf("Robot should be stopped and facing forward\n");
  exit(1);
}

/* This is a function to get the robot a certain distance
   from it's left side to a wall/wave. Get some!
*/
void setDistance(void){
  controller.getSensor(2, &sensors[2]);
  controller.getSensor(3, &sensors[3]);

    if(sensors[2] > 1100){
    controller.steer(0, 0, 0, 0);
    controller.drive(255, 255, 'F', 'F');
    while(sensors[2] > 1000){
      controller.getSensor(2, &sensors[2]);
      printf("Moving Away from wall: %10d\n", sensors[2]);
    }
    usleep(100000);
    controller.steer(90, 90, 90, 90);
    controller.drive(0, 0, 'F', 'F');
  }
  
  if(sensors[2] < 1000){
    controller.steer(0, 0, 0, 0);
    controller.drive(255, 255, 'R', 'R');
    while(sensors[2] < 700){
      controller.getSensor(2, &sensors[2]);
      printf("Moving Closer to wall: %10d\n", sensors[2]);
    }
    usleep(100000);
    controller.steer(90, 90, 90, 90);
    controller.drive(0, 0, 'F', 'F');
  }
}


/* This function computes the angle the robot is to a wall
   from it's left side and then tries to get the robot parallel
   to the wall within a certain range. As Raymond would say...
   Get Squaa!
*/
void getSquare(void){
    controller.getSensor(2, &sensors[2]);
    controller.getSensor(3, &sensors[3]);

    angle = atan2((double)sensors[2]-(double)sensors[3], 900);

    printf("Angle %5f | Sensor1: %10d | Sensor2 %10d\n", angle, sensors[2], sensors[3]);
    

    if(angle > 0.05){
      controller.steer(45,135, 45, 135);
      controller.drive(150, 150, 'F', 'R');
      while(angle > 0.05){
	controller.getSensor(2, &sensors[2]);
	controller.getSensor(3, &sensors[3]);
	angle = atan2((double)sensors[2]-(double)sensors[3], 900);
	printf("Angle %5f | Sensor1: %10d | Sensor2 %10d\n", angle, sensors[2], sensors[3]);
      }
      controller.drive(0, 0, 'F', 'F');
      controller.steer(90, 90, 90, 90);
    }

    if(angle < -0.05){
      controller.steer(45, 135, 45, 135);
      controller.drive(150, 150, 'R', 'F');
      while(angle < -0.05){
	controller.getSensor(2, &sensors[2]);
	controller.getSensor(3, &sensors[3]);
	angle = atan2((double)sensors[2]-(double)sensors[3], 900);
	printf("Angle %5f | Sensor1: %10d | Sensor2 %10d\n", angle, sensors[2], sensors[3]);
      }
      controller.drive(0, 0, 'F', 'F');
      controller.steer(90, 90, 90, 90);  
    }
  controller.steer(90, 90, 90, 90);
  controller.drive(0, 0, 'F', 'F');
}

int main(void){

  //Setup signal handling
  struct sigaction sigHandler;
  sigHandler.sa_handler = killed;
  sigemptyset(&sigHandler.sa_mask);
  sigHandler.sa_flags = 0;
  sigaction(SIGINT, &sigHandler, NULL);



  controller.trim(8, 0, 5, 10);

  setDistance();
  getSquare();
  


  while(1){
    controller.drive(255, 255, 'F', 'F');
    sleep(1);
    setDistance();
    getSquare();

  }
  return 0;
}
