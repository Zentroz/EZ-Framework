#include"Engine/Core/Application.h"
#include"Engine/Events/ApplicationEvents.h"

Application* Application::m_Application = nullptr;

Application::Application() : isRunning(true) {
	m_Application = this;
}

void Application::Shutdown() {
	m_Engine.Shutdown();
}

void Application::CloseApplication() {
	isRunning = false;
}

void Application::OnEvent(Event& event) {
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<ApplicationQuit>([&](ApplicationQuit& e)
		{
			CloseApplication();
			return;
		}
	);

	m_Engine.OnEvent(event);
}

void Application::Init() {
	m_Engine.Init({ 1920, 1080, "Ez-Engine" });
}

void Application::Run() {
	while (isRunning) {
		m_Engine.Tick();
	}
}