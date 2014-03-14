# This directory contains the Controller Library
# and also has a program to test run the controller code

#include directories for includes
include_directories(${GAMMA2_SOURCE_DIR}/serial)
link_directories(${GAMMA2_BINARY_DIR}/serial)

#add controller library to project
add_library(Controller controller.cpp)

#add executable for testing the controller
add_executable(testController testControllerClass.cpp)

target_link_libraries(testController Controller Serial)