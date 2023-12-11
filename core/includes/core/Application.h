#pragma once

#include "core/Events.h"
#include "core/Camera.h"

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

		virtual void onUIDraw() {}

		float camera_pos[3] = { 0, 0, 1 };
		float camera_rotation[3] = { 0, 0, 0 };
		bool perspective_cam = true;
		Camera camera;

	private:
		std::unique_ptr<class Window> m_pWindow;

		EventDispatcher m_event_dispatcher;
		bool m_bCloseWindow = false;
	};

}