#include <controller.hpp>
#include <stdio.h>


enum {S1, S2, S3, S4, S5};
typedef int state_t;

class stateMachine{

private:
  state_t state;

public:
  stateMachine(state_t);
  void advanceState(int);
  state_t checkState(void);
  
};
