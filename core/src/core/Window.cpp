#include "core/Window.h"
#include "core/Logs.h"
#include "core/render/opengl/ShaderProgram.h"
#include "core/render/opengl/VertexBuffer.h"
#include "core/render/opengl/VertexArray.h"
#include "core/render/opengl/IndexBuffer.h"
#include "core/Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

namespace Bunny {

    static bool s_GLFWInit = false;

    GLfloat positions_colors[] = {
        -.5f, -.5f, .0f,  .0f, .0f, 1.f,
        .5f, -.5f, .0f,   1.f, .0f, .0f,
        -.5f, .5f, .0f,   .0f, 1.f, .0f,
        .5f, .5f, .0f,    .0f, 1.f, 1.f
    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1
    };

    const char* vertex_shader =
        R"(#version 460
        layout(location = 0) in vec3 vertex_pos;
        layout(location = 1) in vec3 vertex_col;
        uniform mat4 model_matrix;
        uniform mat4 view_projection_matrix;
        out vec3 col;
        void main() {
           col = vertex_col;
           gl_Position = view_projection_matrix * model_matrix * vec4(vertex_pos, 1.);
        }
        )";

    const char* fragment_shader =
        R"(#version 460
        in vec3 col;
        out vec4 frag_color;
        void main() {
           frag_color = vec4(col, 1.);
        }
        )";

    std::unique_ptr<ShaderProgram> p_shader_program;
    std::unique_ptr<VertexBuffer> p_vbo;
    std::unique_ptr<IndexBuffer> p_ibo;
    std::unique_ptr<VertexArray> p_vao;
    
    float scale[3] = { 1, 1, 1 };
    float rotate[3] = { 0, 0, 0 };
    float translate[3] = { 0, 0, 0 };

    float camera_pos[3] = { 0, 0, 1 };
    float camera_rotation[3] = { 0, 0, 0 };
    bool perspective_cam = false;
    Camera camera;

	Window::Window(std::string title, const unsigned int width, const unsigned int height)
        :m_data({ std::move(title), width, height })
	{
		int resultCode = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
	}

	Window::~Window() {
		Shutdown();
	}

	int Window::init() {
        LOG_INFO("Create Window '{0}', size: {1}x{2}", m_data.title, m_data.width, m_data.height);
        
        if (!s_GLFWInit) {
            if (!glfwInit()) {
                LOG_CRIT("Failed initialized GLFW");
                return -1;
            }
            s_GLFWInit = true;
        }

        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);

        if (!m_pWindow)
        {
            LOG_CRIT("Failed create window {0}", m_data.title);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(m_pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRIT("Failed to initialize GLAD");
            return -3;
        }

        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetWindowSizeCallback(m_pWindow, 
            [](GLFWwindow* pWindow, int width, int height) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;

                EventWindowResize event(width, height);
                data.eventCallbackFn(event);
            });
        
        glfwSetCursorPosCallback(m_pWindow,
            [](GLFWwindow* pWindow, double x, double y) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);
            });

        glfwSetWindowCloseCallback(m_pWindow,
            [](GLFWwindow* pWindow) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

                EventWindowClose event;
                data.eventCallbackFn(event);
            });

        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height) {
                glViewport(0, 0, width, height);
            });

        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled()) {
            return false;
        }

        BufferLayout buffer_layout_vec3 {
            ShaderDataType::FLoat3,
            ShaderDataType::FLoat3
        };

        p_vao = std::make_unique<VertexArray>();
        p_vbo = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), buffer_layout_vec3);
        p_ibo = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

        p_vao->add_vertex_buffer(*p_vbo);
        p_vao->set_index_buffer(*p_ibo);

        return 0;
	}

	void Window::Shutdown() {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
	}

	void Window::Update() {
        glClearColor(
            m_background_color[0],
            m_background_color[1],
            m_background_color[2],
            m_background_color[3]);

        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(350, 350));
        ImGui::Begin("Properties");

        ImGui::ColorEdit4("Color", m_background_color);
        ImGui::SliderFloat3("Scale", scale, .0f, 2.f);
        ImGui::SliderFloat3("Rotate", rotate, .0f, 360.f);
        ImGui::SliderFloat3("Translate", translate, -1.f, 1.f);

        ImGui::SliderFloat3("Camera Position", camera_pos, -10.f, 10.f);
        ImGui::SliderFloat3("Camera Rotation", camera_rotation, 0, 360.f);
        ImGui::Checkbox("Camera Perspective", &perspective_cam);

        p_shader_program->bind();

        glm::mat4 scale_matrix(scale[0], 0,        0,        0,
                              0,         scale[1], 0,        0,
                              0,         0,        scale[2], 0,
                              0,         0,        0,        1);

        float rotate_in_rad_x = glm::radians(rotate[0]);
        glm::mat4 rotate_matrix_x(1, 0, 0, 0,
            0, cos(rotate_in_rad_x), sin(rotate_in_rad_x), 0,
            0, -sin(rotate_in_rad_x), cos(rotate_in_rad_x), 0,
            0, 0, 0, 1);

        float rotate_in_rad_y = glm::radians(rotate[1]);
        glm::mat4 rotate_matrix_y(cos(rotate_in_rad_y), 0, -sin(rotate_in_rad_y), 0,
            0, 1, 0, 0,
            sin(rotate_in_rad_y), 0, cos(rotate_in_rad_y), 0,
            0, 0, 0, 1);

        float rotate_in_rad_z = glm::radians(rotate[2]);
        glm::mat4 rotate_matrix_z( cos(rotate_in_rad_z), sin(rotate_in_rad_z), 0, 0,
                             -sin(rotate_in_rad_z), cos(rotate_in_rad_z), 0, 0,
                              0,                  0,                  1, 0,
                              0,                  0,                  0, 1);

        glm::mat4 translate_matrix(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          translate[0], translate[1], translate[2], 1);

        glm::mat4 rotate_matrix = rotate_matrix_x * rotate_matrix_y * rotate_matrix_z;

        glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;

        p_shader_program->setMat4("model_matrix", model_matrix);

        camera.set_position_rotation(glm::vec3(camera_pos[0], camera_pos[1], camera_pos[2]),
            glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));

        camera.set_projection_mode(perspective_cam ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Ortographic);

        p_shader_program->setMat4("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());

        p_vao->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_vao->get_indices_count()), GL_UNSIGNED_INT, nullptr);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
	}

}