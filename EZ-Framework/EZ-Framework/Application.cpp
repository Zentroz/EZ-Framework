#include"Application.h"
#include<string>

Application::Application(int width, int height, const char* title)
	: window(width, height, title), renderer(window.GetHwnd()) {
	Time::Start();
	scriptManager.RegisterWrapperFunctions(window.GetInput(), &registry);
	scriptManager.LoadScript("Scripts/test.lua");
}

Application::~Application() {
	window.Shutdown();
}

void Application::Run() {
	while (window.isRunning) {
		time.Update();
		window.Run();
		scriptManager.Update();
		renderer.Render();

		float3 camPos = renderer.GetCamera().GetPosition();

		if (window.GetInput().GetKey(Input::KeyCode::W)) {
			renderer.GetCamera().SetPosition(camPos + float3(0, 0, 1) * Time::deltaTime);
		}
		else if (window.GetInput().GetKey(Input::KeyCode::S)) {
			renderer.GetCamera().SetPosition(camPos + float3(0, 0, -1) * Time::deltaTime);
		}

		if (window.GetInput().GetKey(Input::KeyCode::A)) {
			renderer.GetCamera().SetPosition(camPos + float3(-1, 0, 0) * Time::deltaTime);
		}
		else if (window.GetInput().GetKey(Input::KeyCode::D)) {
			renderer.GetCamera().SetPosition(camPos + float3(1, 0, 0) * Time::deltaTime);
		}

		if (window.GetInput().GetKey(Input::KeyCode::Q)) {
			renderer.GetCamera().SetPosition(camPos + float3(0, 1, 0) * Time::deltaTime);
		}
		else if (window.GetInput().GetKey(Input::KeyCode::E)) {
			renderer.GetCamera().SetPosition(camPos + float3(0, -1, 0) * Time::deltaTime);
		}
		
		if (window.GetInput().GetKey(Input::KeyCode::F)) {
			renderer.GetCamera().FlipForward(1);
		}
		else if (window.GetInput().GetKey(Input::KeyCode::C)) {
			renderer.GetCamera().FlipForward(-1);
		}
	}
}