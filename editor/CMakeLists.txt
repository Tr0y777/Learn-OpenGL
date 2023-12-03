cmake_minimum_required(VERSION 3.26)

set(EDITOR_NAME editor)

add_executable(${EDITOR_NAME}
	src/Source.cpp
)

target_link_libraries(${EDITOR_NAME} core ImGui)
target_compile_features(${EDITOR_NAME} PUBLIC cxx_std_17)

set_target_properties(${EDITOR_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)