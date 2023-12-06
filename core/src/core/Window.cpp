#include "core/Window.h"
#include "core/Logs.h"
#include "core/render/opengl/ShaderProgram.h"
#include "core/render/opengl/VertexBuffer.h"
#include "core/render/opengl/VertexArray.h"
#include "core/render/opengl/IndexBuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

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
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_pos;"
        "layout(location = 1) in vec3 vertex_col;"
        "out vec3 col;"
        "void main() {"
        "   col = vertex_col;"
        "   gl_Position = vec4(vertex_pos, 1.);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 col;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(col, 1.);"
        "}";

    std::unique_ptr<ShaderProgram> p_shader_program;
    std::unique_ptr<VertexBuffer> p_vbo;
    std::unique_ptr<IndexBuffer> p_ibo;
    std::unique_ptr<VertexArray> p_vao;
    

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
        ImGui::SetNextWindowSize(ImVec2(350, 75));
        ImGui::Begin("Properties");

        ImGui::ColorEdit4("Color", m_background_color);

        static bool test = false;
        ImGui::Checkbox("test", &test);

        p_shader_program->bind();
        p_vao->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_vao->get_indices_count()), GL_UNSIGNED_INT, nullptr);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
	}

}