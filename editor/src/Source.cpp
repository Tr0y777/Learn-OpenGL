#include <core/Application.h>
#include <memory>
#include <iostream>

class MyApp : public Bunny::Application {
	virtual void Update() override {
		// std::cout << "Update frame: " << frame++ << std::endl;
	}

	int frame = 0;
};

int main() {
	auto myApp = std::make_unique<MyApp>();

	int returnCode = myApp->Start(800, 600, "My App");

	return returnCode;
}