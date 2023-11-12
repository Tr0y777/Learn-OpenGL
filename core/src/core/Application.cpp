#include "core/Application.h"
#include "core/logs.h"
#include "core/Window.h"

namespace Bunny {

	Application::Application() {
		LOG_INFO("Start App");
	}

	Application::~Application() {
		LOG_INFO("Close App");
	}

	int Application::Start(unsigned int window_width, unsigned int window_height, const char* title) {
		m_Window = std::make_unique<Window>(title, window_width, window_height);

		while (true) {
			Update();
			m_Window->Update();
		}

        return 0;
	}
}