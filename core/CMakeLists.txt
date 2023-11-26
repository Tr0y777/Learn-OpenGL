cmake_minimum_required(VERSION 3.26)

set(ENGINE_NAME core)

set(ENGINE_PUBLIC_INCLUDES
	includes/core/Application.h
	includes/core/Logs.h
	includes/core/Events.h
)

set(ENGINE_PRIVATE_INCLUDES
	src/core/Window.h
)

set(ENGINE_PUBLIC_SOURCE
	src/core/Application.cpp
	src/core/WIndow.cpp
)

add_library(${ENGINE_NAME} STATIC
	${ENGINE_PUBLIC_SOURCE}
	${ENGINE_PRIVATE_INCLUDES}
	${ENGINE_PUBLIC_INCLUDES}
)

target_include_directories(${ENGINE_NAME} PUBLIC includes)
target_include_directories(${ENGINE_NAME} PRIVATE src)
target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_NAME} PRIVATE glfw)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_NAME} PRIVATE spdlog)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_NAME} PRIVATE glad)

set(IMGUI_INCLUDES
	../external/imgui/imgui.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/imgui_demo.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
	../external/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${ENGINE_NAME} PRIVATE ImGui)