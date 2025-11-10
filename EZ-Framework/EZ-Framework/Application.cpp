#include"Application.h"
#include<string>

Application::Application(int width, int height, const char* title)
	: window(width, height, title) {
	Time::Start();
}

Application::~Application() {
	window.Shutdown();
}

void Application::Run() {
	time.Update();
	window.Run();


	while (window.isRunning) {
		time.Update();
		window.Run();

		SetWindowTextA(window.GetHwnd(), (std::string("EZ Framework - ") + "Time: " + std::to_string(time.time) +
			", Delta Time: " + std::to_string(Time::deltaTime) +
			", FPS: " + std::to_string(time.fps)).c_str()
		);
	}
}