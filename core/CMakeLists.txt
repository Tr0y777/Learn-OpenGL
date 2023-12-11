cmake_minimum_required(VERSION 3.26)

set(ENGINE_NAME core)

set(ENGINE_PUBLIC_INCLUDES
	includes/core/Application.h
	includes/core/Logs.h
	includes/core/Events.h
	includes/core/Camera.h
	includes/core/Keys.h
	includes/core/Input.h
)

set(ENGINE_PRIVATE_INCLUDES
	src/core/Window.h
	src/core/Modules/UIModule.h
	src/core/render/OpenGL/ShaderProgram.h
	src/core/render/OpenGL/VertexBuffer.h
	src/core/render/OpenGL/IndexBuffer.h
	src/core/render/OpenGL/VertexArray.h
	src/core/render/OpenGL/Render_OpenGL.h
)

set(ENGINE_PRIVATE_SOURCES
	src/core/Application.cpp
	src/core/Window.cpp
	src/core/Camera.cpp
	src/core/Input.cpp
	src/core/Modules/UIModule.cpp
	src/core/render/OpenGL/ShaderProgram.cpp
	src/core/render/OpenGL/VertexBuffer.cpp
	src/core/render/OpenGL/IndexBuffer.cpp
	src/core/render/OpenGL/VertexArray.cpp
	src/core/render/OpenGL/Render_OpenGL.cpp
)

set(ENGINE_ALL_SOURCES
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PRIVATE_INCLUDES}
	${ENGINE_PRIVATE_SOURCES}
)

add_library(${ENGINE_NAME} STATIC
	${ENGINE_ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${ENGINE_ALL_SOURCES}
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

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_NAME} PRIVATE glm)

set(IMGUI_INCLUDES
	../external/imgui/imgui.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_OpenGL3.h
)

set(IMGUI_SOURCES
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_OpenGL3.cpp
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
target_link_libraries(ImGui PRIVATE glad glfw glm)

target_link_libraries(${ENGINE_NAME} PRIVATE ImGui)