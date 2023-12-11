#include "core/Application.h"
#include "core/logs.h"
#include "core/Window.h"
#include "core/Camera.h"
#include "core/Input.h"
#include "core/modules/UIModule.h"
#include "core/render/OpenGL/ShaderProgram.h"
#include "core/render/OpenGL/VertexBuffer.h"
#include "core/render/OpenGL/VertexArray.h"
#include "core/render/OpenGL/IndexBuffer.h"
#include "core/render/OpenGL/Render_OpenGL.h"

#include <imgui/imgui.h>
#include <GLFW/glfw3.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

namespace Bunny {

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
    float m_background_color[4] = { .4f, .4f, .4f, .0f };

	Application::Application() {
		LOG_INFO("Start App");
	}

	Application::~Application() {
		LOG_INFO("Close App");
	}

	int Application::Start(unsigned int window_width, unsigned int window_height, const char* title) {
		m_pWindow = std::make_unique<Window>(title, window_width, window_height);

		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event) {
				// LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
			});

		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event) {
				LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
			});

		m_event_dispatcher.add_event_listener<EventWindowClose>(
			[&](EventWindowClose& event) {
				LOG_INFO("[WindowClose]");
				m_bCloseWindow = true;
			});
        
        m_event_dispatcher.add_event_listener<EventKeyPressed>(
			[&](EventKeyPressed& event) {
                if (event.key_code <= KeyCode::KEY_Z) {
                    if (event.repeat) {
                        LOG_INFO("[EventKeyPressed]: {0} repeated", static_cast<char>(event.key_code));
                    }
                    else {
                        LOG_INFO("[EventKeyPressed]: {0}", static_cast<char>(event.key_code));
                    }
                }
                Input::PressKey(event.key_code);
			});
        
        m_event_dispatcher.add_event_listener<EventKeyReleased>(
			[&](EventKeyReleased& event) {
                if (event.key_code <= KeyCode::KEY_Z) {
                    LOG_INFO("[EventKeyReleased]: {0}", static_cast<char>(event.key_code));
                }
                Input::ReleaseKey(event.key_code);
			});

		m_pWindow->set_event_callback(
			[&](BaseEvent& event) {
				m_event_dispatcher.dispatch(event);
			});

        //-----------------------------------------------------------------------------------------------------//
        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled()) {
            return false;
        }

        BufferLayout buffer_layout_vec3{
            ShaderDataType::FLoat3,
            ShaderDataType::FLoat3
        };

        p_vao = std::make_unique<VertexArray>();
        p_vbo = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), buffer_layout_vec3);
        p_ibo = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

        p_vao->add_vertex_buffer(*p_vbo);
        p_vao->set_index_buffer(*p_ibo);
        //-----------------------------------------------------------------------------------------------------//

		while (!m_bCloseWindow) {
            Render_OpenGL::set_clear_color(
                m_background_color[0],
                m_background_color[1],
                m_background_color[2],
                m_background_color[3]);

            Render_OpenGL::clear();

            p_shader_program->bind();

            glm::mat4 scale_matrix(scale[0], 0, 0, 0,
                0, scale[1], 0, 0,
                0, 0, scale[2], 0,
                0, 0, 0, 1);

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
            glm::mat4 rotate_matrix_z(cos(rotate_in_rad_z), sin(rotate_in_rad_z), 0, 0,
                -sin(rotate_in_rad_z), cos(rotate_in_rad_z), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);

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

            Render_OpenGL::draw(*p_vao);

            UIModule::onUIDrawBegin();

            bool show = true;
            UIModule::ShowExampleAppDockSpace(&show);
            ImGui::Begin("Properties");
            ImGui::ColorEdit4("Color", m_background_color);
            ImGui::SliderFloat3("Scale", scale, .0f, 2.f);
            ImGui::SliderFloat3("Rotate", rotate, .0f, 360.f);
            ImGui::SliderFloat3("Translate", translate, -1.f, 1.f);

            ImGui::SliderFloat3("Camera Position", camera_pos, -10.f, 10.f);
            ImGui::SliderFloat3("Camera Rotation", camera_rotation, 0, 360.f);
            ImGui::Checkbox("Camera Perspective", &perspective_cam);
            ImGui::End();

            onUIDraw();

            UIModule::onUIDrawEnd();

			m_pWindow->Update();
			Update();
		}
		m_pWindow = nullptr;

        return 0;
	}
}