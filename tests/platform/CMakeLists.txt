include_directories(${GAMMA2_SOURCE_DIR}/controller ${GAMMA2_SOURCE_DIR}/serial)
link_directories(${GAMMA2_BINARY_DIR}/controller ${GAMMA2_BINARY_DIR}/serial)

add_executable(platformTest platformTest.cpp)
target_link_libraries(platformTest Controller Serial)

add_executable(platformPositionTest platformPositionTest.cpp)
target_link_libraries(platformPositionTest Controller Serial)