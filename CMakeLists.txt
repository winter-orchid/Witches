cmake_minimum_required(VERSION 3.16)
project(TurnBasedRoguelite C)

set(CMAKE_C_STANDARD 11)

find_package(raylib REQUIRED)

add_executable(${PROJECT_NAME} main.c
        header.h
        menu.c
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets)

target_link_libraries(${PROJECT_NAME} raylib m pthread dl rt GL X11)