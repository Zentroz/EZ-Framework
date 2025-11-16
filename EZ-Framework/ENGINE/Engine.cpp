#include"Engine/Engine.h"

Engine::Engine() {
}

void Engine::Init(int width, int height, const char* title) {
	backend.Init(width, height, title);
	renderer.Init(backend.GetDevice(), backend.GetRenderContext());
}

void Engine::Shutdown() {
	renderer.Shutdown();
	backend.Shutdown();
}

void Engine::Frame() {
	bool run = true;

	while (run) {
		time.Update();
		run = backend.Run();

		renderer.InitRender(backend.GetMainRenderTarget());
		renderer.Render();
		renderer.EndRender();

		backend.PresentFrame();

		if (run == false) break;
	}
}