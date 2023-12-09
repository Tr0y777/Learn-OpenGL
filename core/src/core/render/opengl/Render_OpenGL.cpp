#include "core/render/OpenGL/Render_OpenGL.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "core/Logs.h"

namespace Bunny {

	bool Render_OpenGL::init(GLFWwindow* pWindow) {
		glfwMakeContextCurrent(pWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOG_CRIT("Failed to initialize GLAD");
			return false;
		}

		LOG_INFO("OpenGL context initialized");
		LOG_INFO("  Vendor: {0}", get_vendor_str());
		LOG_INFO("  Render: {0}", get_render_str());
		LOG_INFO("  Version: {0}", get_version_str());

		return true;
	}

	void Render_OpenGL::draw(const VertexArray& vertex_array) {
		vertex_array.bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.get_indices_count()), GL_UNSIGNED_INT, nullptr);
	}

	void Render_OpenGL::set_clear_color(const float r, const float g, const float b, const float a) {
		glClearColor(r, g, b, a);
	}

	void Render_OpenGL::clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Render_OpenGL::set_viewport(const unsigned int width, const unsigned int height, const unsigned int left_offset, const unsigned int bottom_offset) {
		glViewport(left_offset, bottom_offset, width, height);
	}

	const char* Render_OpenGL::get_vendor_str() {
		return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	}

	const char* Render_OpenGL::get_render_str() {
		return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	}

	const char* Render_OpenGL::get_version_str() {
		return reinterpret_cast<const char*>(glGetString(GL_VERSION));
	}

}