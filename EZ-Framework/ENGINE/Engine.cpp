#include"Engine/Engine.h"

Engine::Engine() {
}

void Engine::Init(int width, int height, const char* title) {
	backend.Init(width, height, title);
	renderer.Init(backend.GetDevice(), 
		backend.GetRenderContext(), 
		backend.CreateDepthStencilState(D3D11_COMPARISON_LESS_EQUAL), 
		backend.CreateRasteriserState(D3D11_CULL_BACK, D3D11_FILL_SOLID)
	);

	scriptManager.Init(backend.GetInput(), renderer.GetCamera(), &registry);

	time.Start();

	scriptManager.LoadScript("Assets/Scripts/test.lua");
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

		scriptManager.Update();

		renderer.InitRender(backend.GetMainRenderTarget());
		renderer.Render(registry.CreateRenderList());
		renderer.EndRender();

		backend.PresentFrame();

		if (run == false) break;
	}
}