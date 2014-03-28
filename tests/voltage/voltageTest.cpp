#include <controller.cpp>
#include <stdio.h>

Controller* controller = new Controller("/dev/ttyACM1");

int main(void){

  float voltage = 0;

  controller->voltage(&voltage);

  printf("The battery voltage should be %.2f\n", voltage);

  controller->close();

  return 0;
}
