#include <core/Application.h>
#include <memory>
#include <iostream>
#include <imgui/imgui.h>

#include <core/Input.h>
#include <core/Application.h>

class BunnyEngineEditor : public Bunny::Application {
	virtual void Update() override {
		bool move_camera = false;
		glm::vec3 movement_delta{ 0, 0, 0 };
		glm::vec3 rotation_delta{ 0, 0, 0 };

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_W)) {
			movement_delta.x += .05f;
			move_camera = true;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_S)) {
			movement_delta.x -= .05f;
			move_camera = true;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_A)) {
			movement_delta.y -= .05f;
			move_camera = true;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_D)) {
			movement_delta.y += .05f;
			move_camera = true;
		}
		
		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_E)) {
			movement_delta.z += .05f;
			move_camera = true;
		}
		
		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_Q)) {
			movement_delta.z -= .05f;
			move_camera = true;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_UP)) {
			rotation_delta.y -= .5f;
			move_camera = true;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_DOWN)) {
			rotation_delta.y += .5f;
			move_camera = true;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_LEFT)) {
			rotation_delta.z += .5f;
			move_camera = true;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_RIGHT)) {
			rotation_delta.z -= .5f;
			move_camera = true;
		}
		
		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_P)) {
			rotation_delta.x += .5f;
			move_camera = true;
		}
		
		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_O)) {
			rotation_delta.x -= .5f;
			move_camera = true;
		}
		if (move_camera)
			camera.add__movement_and_rotation(movement_delta, rotation_delta);
	}

	virtual void onUIDraw() override {
		ImGui::Begin("Editor");
		ImGui::SliderFloat3("Camera Position", camera_pos, -10.f, 10.f);
		ImGui::SliderFloat3("Camera Rotation", camera_rotation, 0, 360.f);
		ImGui::Checkbox("Camera Perspective", &perspective_cam);
		ImGui::End();
	}

	int frame = 0;
};

int main() {
	auto Editor = std::make_unique<BunnyEngineEditor>();

	int returnCode = Editor->Start(1024, 768, "BunnyEngine Editor");

	return returnCode;
}