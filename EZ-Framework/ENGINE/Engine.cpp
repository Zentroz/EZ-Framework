#include"Engine/Engine.h"

void Engine::Init(int width, int height, const char* title) {
	window.Init(width, height, title);
	renderer.Init(window.GetHwnd());

	Entity a = registry.CreateEntity();

	ScriptComponent* script = new ScriptComponent("Scripts/test.lua");

	//registry.AddComponent<ScriptComponent>(b, script);

	scriptManager.Init(window.GetInput(), &registry, &renderer.GetCamera());
	scriptManager.Start(&registry);
	sqrt(360);
}

void Engine::Shutdown() {
	renderer.Shutdown();
	window.Shutdown();
}

void Engine::Frame() {
	while (window.isRunning) {
		time.Update();
		window.Run();
		scriptManager.Update(&registry);
		renderer.Render();
	}
}