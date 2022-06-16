cmake_minimum_required(VERSION 3.23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/linux-$<LOWER_CASE:${CMAKE_BUILD_TYPE}>)
project(g600-mouse-controller)

add_executable(${PROJECT_NAME} g600.c)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)