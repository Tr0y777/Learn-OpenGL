#pragma once

#include <memory>

namespace Bunny {

	class Application {
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int Start(unsigned int window_width, unsigned int window_height, const char* title);

		virtual void Update() {}

	private:
		std::unique_ptr<class Window> m_Window;
	};

}