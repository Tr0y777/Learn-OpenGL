#include <core/Application.h>
#include <memory>
#include <iostream>
#include <imgui/imgui.h>

#include <core/Input.h>
#include <core/Application.h>

class BunnyEngineEditor : public Bunny::Application {
	virtual void Update() override {
		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_W)) {
			camera_pos[2] -= .01f;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_S)) {
			camera_pos[2] += .01f;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_A)) {
			camera_pos[0] -= .01f;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_D)) {
			camera_pos[0] += .01f;
		}
		
		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_E)) {
			camera_pos[1] += .01f;
		}
		
		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_Q)) {
			camera_pos[1] -= .01f;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_UP)) {
			camera_rotation[0] += .5f;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_DOWN)) {
			camera_rotation[0] -= .5f;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_LEFT)) {
			camera_rotation[1] += .5f;
		}

		if (Bunny::Input::isKeyPressed(Bunny::KeyCode::KEY_RIGHT)) {
			camera_rotation[1] -= .5f;
		}
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