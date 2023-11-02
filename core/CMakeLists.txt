cmake_minimum_required(VERSION 3.26)

set(ENGINE_NAME core)

add_library(${ENGINE_NAME} STATIC
	src/core/utils/Source.cpp
	includes/core/utils/test.h
)

target_include_directories(${ENGINE_NAME} PUBLIC includes)
target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_17)