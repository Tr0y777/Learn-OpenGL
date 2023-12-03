cmake_minimum_required(VERSION 3.26)

set(PROJ_NAME BunnyEngine)
project(${PROJ_NAME})

add_subdirectory(core)
add_subdirectory(editor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT editor)