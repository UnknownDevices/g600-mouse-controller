cmake_minimum_required(VERSION 3.23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/linux-$<LOWER_CASE:${CMAKE_BUILD_TYPE}>)
project(g600-mouse-controller)

add_executable(${PROJECT_NAME} g600.c)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND sudo -A chown :input ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(CODE "
    execute_process(
        COMMAND sudo -A chown :input ${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}
        )
    "
)