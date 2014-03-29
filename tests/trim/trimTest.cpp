#include <controller.hpp>

Controller controller("/dev/ttyACM0");

int main(void){


  controller.trim(7, 0, 7, 0);

  controller.steer(90, 90, 90, 90);

  controller.drive(255, 255, 'R', 'R');
  sleep(10);
  controller.drive(0, 0, 'F', 'F');

  return 0;
}
