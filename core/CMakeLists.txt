cmake_minimum_required(VERSION 3.26)

set(ENGINE_NAME core)

set(ENGINE_PUBLIC_INCLUDES
	includes/core/Application.h
	includes/core/log.h
)

set(ENGINE_PUBLIC_SOURCE
	src/core/Application.cpp
)

add_library(${ENGINE_NAME} STATIC
	${ENGINE_PUBLIC_SOURCE}
	${ENGINE_PUBLIC_INCLUDES}
)

target_include_directories(${ENGINE_NAME} PUBLIC includes)
target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_NAME} PRIVATE glfw)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_NAME} PRIVATE spdlog)