#include <controller.hpp>

Controller controller("/dev/ttyACM0");

int main(void){


  controller.trim(15, 0, 5, 15);

  controller.steer(0, 0, 0, 0);

  return 0;
}
