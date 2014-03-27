#include <controller.cpp>
#include <stdio.h>
#include <signal.h>

Controller controller("/dev/ttyACM0");

void killed(int sig){
  printf("\n\nProgram Killed!\n");
  controller.close();
  printf("Robot should be stopped and facing forward\n");
  exit(1);
}

int main(void){

  //Setup signal handling
  struct sigaction sigHandler;
  sigHandler.sa_handler = killed;
  sigemptyset(&sigHandler.sa_mask);
  sigHandler.sa_flags = 0;
  sigaction(SIGINT, &sigHandler, NULL);

  controller.deployCamera(3, 15);

  printf("Sleeping, then closing controller\n\n");
  sleep(500);

  controller.close();
  printf("Closed!\n\n");
  return 0;
}
