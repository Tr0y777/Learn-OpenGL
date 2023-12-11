#include "core/Window.h"
#include "core/Logs.h"
#include "core/modules/UIModule.h"
#include "core/render/OpenGL/Render_OpenGL.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace Bunny {

    Window::Window(std::string title, const unsigned int width, const unsigned int height)
        :m_data({ std::move(title), width, height })
    {
        int resultCode = init();
    }

    Window::~Window() {
        Shutdown();
    }

    int Window::init() {
        LOG_INFO("Create Window '{0}', size: {1}x{2}", m_data.title, m_data.width, m_data.height);

        glfwSetErrorCallback([](int error_code, const char* description) {
            LOG_CRIT("GLFW error: {0}", description);
            });

        if (!glfwInit()) {
            LOG_CRIT("Failed initialized GLFW");
            return -1;
        }

        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);

        if (!m_pWindow)
        {
            LOG_CRIT("Failed create window {0}", m_data.title);
            return -2;
        }

        if (!Render_OpenGL::init(m_pWindow)) {
            LOG_CRIT("Failed to initialize OpenGL render");
            return -3;
        }

        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetKeyCallback(m_pWindow,
            [](GLFWwindow* pWindow, int key, int scancode, int action, int mods) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                switch (action) {
                case GLFW_PRESS: {
                    EventKeyPressed event(static_cast<KeyCode>(key), false);
                    data.eventCallbackFn(event);
                    break;
                }

                case GLFW_RELEASE: {
                    EventKeyReleased event(static_cast<KeyCode>(key));
                    data.eventCallbackFn(event);
                    break;
                }

                case GLFW_REPEAT: {
                    EventKeyPressed event(static_cast<KeyCode>(key), true);
                    data.eventCallbackFn(event);
                    break;
                }
                }
            });

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
                Render_OpenGL::set_viewport(width, height);
            }
        );

        UIModule::onWindowCreate(m_pWindow);

        return 0;
    }

    void Window::Shutdown() {
        UIModule::onWindowClose();
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::Update() {
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }

}