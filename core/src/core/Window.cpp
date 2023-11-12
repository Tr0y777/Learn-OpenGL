#include "core/Window.h"
#include "core/Logs.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Bunny {

    static bool s_GLFWInit = false;

	Window::Window(std::string title, const unsigned int width, const unsigned int height)
		:m_title(std::move(title)),
			m_width(width),
			m_height(height) 
	{
		int resultCode = Init();
	}

	Window::~Window() {
		Shutdown();
	}

	int Window::Init() {
        LOG_INFO("Create Window '{0}', size: {1}x{2}", m_title, m_width, m_height);
        
        if (!s_GLFWInit) {
            if (!glfwInit()) {
                LOG_CRIT("Failed initialized GLFW");
                return -1;
            }
            s_GLFWInit = true;
        }

        m_Window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

        if (!m_Window)
        {
            LOG_CRIT("Failed create window {0}", m_title);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRIT("Failed to initialize GLAD");
            return -3;
        }

        return 0;
	}

	void Window::Shutdown() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
	}

	void Window::Update() {
        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
	}

}