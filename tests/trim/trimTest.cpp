#include <controller.hpp>

Controller controller("/dev/ttyACM0");

int main(void){


  controller.trim(5, -5, 5, 0);

  controller.steer(90, 90, 90, 90);
  sleep(5);
  controller.steer(180, 180, 180, 180);
  sleep(5);
  controller.steer(0, 0, 0, 0);
  sleep(5);
  controller.steer(90, 90, 90, 90);


  controller.drive(255, 255, 'F', 'F');
  sleep(10);
  controller.drive(0, 0, 'F', 'F');
  controller.close();

  return 0;
}
