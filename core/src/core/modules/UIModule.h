#pragma once

struct GLFWwindow;

namespace Bunny {

	class UIModule {
	public:
		static void onWindowCreate(GLFWwindow* pWindow);
		static void onWindowClose();
		static void onUIDrawBegin();
		static void onUIDrawEnd();
		static void ShowExampleAppDockSpace(bool* p_open);
	};

}