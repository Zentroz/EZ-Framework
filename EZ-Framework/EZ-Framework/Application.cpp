#include"Application.h"
#include<string>

Application::Application(int width, int height, const char* title)
	: window(width, height, title), renderer(window.GetHwnd()) {
	Time::Start();
}

Application::~Application() {
	window.Shutdown();
}

void Application::Run() {
	while (window.isRunning) {
		time.Update();
		window.Run();
		renderer.Render();
	}
}