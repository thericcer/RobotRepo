#include <syslog.h>
#include <stdio.h>
#include <unistd.h>

int main(void){


  openlog("Robot", LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_LOCAL0);

  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO), "INFO");

  sleep(5);
  
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_WARNING), "Warning");

  sleep(5);
  
  syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR), "Super hello!");

  closelog();
}
