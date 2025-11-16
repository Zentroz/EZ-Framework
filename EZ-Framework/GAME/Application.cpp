#include"Application.h"

Application::Application(int width, int height, const char* title) : engine() {
	engine.Init(width, height, title);
}

Application::~Application() {
	engine.Shutdown();
}

void Application::Run() {
	engine.Frame();
}