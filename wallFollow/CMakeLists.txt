include_directories(${GAMMA2_SOURCE_DIR}/controller ${GAMMA2_SOURCE_DIR}/serial)
link_directories(${GAMMA2_BINARY_DIR}/controller ${GAMMA2_BINARY_DIR}/serial)

add_executable(wallFollow wallFollow.cpp)
target_link_libraries(wallFollow Controller Serial)

add_executable(waveCount waveCount.cpp)
target_link_libraries(waveCount Controller Serial)