include_directories(${GAMMA2_SOURCE_DIR}/controller ${GAMMA2_SOURCE_DIR}/serial)
link_directories(${GAMMA2_BINARY_DIR}/controller ${GAMMA2_BINARY_DIR}/serial)

add_executable(pusherTest pusherTest.cpp)
target_link_libraries(pusherTest Controller Serial)
