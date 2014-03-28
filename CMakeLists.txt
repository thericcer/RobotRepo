# This directory contains all necesary code 

# for communication for the Gamma 2 Demo

cmake_minimum_required(VERSION 2.6)

project(GAMMA2)

#Add Subdirectories Here

add_subdirectory(serial)
add_subdirectory(controller)
add_subdirectory(wallFollow)
add_subdirectory(tests/platform)
add_subdirectory(tests/hook)
add_subdirectory(tests/trim)
add_subdirectory(tests/cameraBoom)
add_subdirectory(tests/pingSpeed)
add_subdirectory(tests/deploy)
add_subdirectory(tests/pusher)
add_subdirectory(tests/voltage)
add_subdirectory(tests/log)