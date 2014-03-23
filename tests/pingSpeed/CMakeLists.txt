include_directories(${GAMMA2_SOURCE_DIR}/controller ${GAMMA2_SOURCE_DIR}/serial)
link_directories(${GAMMA2_BINARY_DIR}/controller ${GAMMA2_BINARY_DIR}/serial)

add_executable(pingSpeed pingSpeed.cpp)
target_link_libraries(pingSpeed Controller Serial)
