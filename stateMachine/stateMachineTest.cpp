#include "stateMachineTest.hpp"

stateMachine::stateMachine(state_t s) : state(S1){
  state = s;
}

void stateMachine::advanceState(int noWall){
  if (noWall){
    if (state == S3){
      state = S5;
      return;
    }
  }

  if(state == S1){
    state = S2;
    return;
  }

  if(state == S2){
    state = S3;
    return;
  }

  //NOT DONE HERE!
  
}

state_t stateMachine::checkState(void){
  return state;
}

int main(void){

  printf("A Program to demonstrate State Machine Cycle\n\n");
  getchar();
  stateMachine machine(S3);
  printf("Current State %d\n", machine.checkState());
  getchar();
  machine.advanceState(1);
  printf("New State %d\n", machine.checkState());
  return 0;
}
