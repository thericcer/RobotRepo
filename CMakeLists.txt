# This directory contains all necesary code 
# for communication for the Gamma 2 Demo

cmake_minimum_required(VERSION 2.6)

project(GAMMA2)

#Add Subdirectories Here

add_subdirectory(serial)
add_subdirectory(controller)
add_subdirectory(wallFollow)
add_subdirectory(stateMachine)
add_subdirectory(platform)
add_subdirectory(hook)
add_subdirectory(tests)