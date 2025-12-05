#include"Engine.h"

namespace ENGINE
{
	Engine::EngineContext Engine::ctx = {};

	Engine::Engine() {
		Engine::ctx.SetEngine(this);
	}

	void Engine::Init(EngineInitData initData) {
		backend.Init({ initData.width, initData.height, initData.title });

		backend.CreateRenderTargetBackBuffer();
		backend.CreateRenderTargetTexture();

		ImGuiLayer::Setup();

		resourceManager.SetDevice(backend.GetDevice());

		renderer.Init({ backend.GetDevice(), backend.GetRenderContext(), &registry, &assetManager, &resourceManager, initData.width, initData.height });

		time.Start();

		SCRIPT::ScriptRuntime::SetRegistry(&registry);
		SCRIPT::ScriptRuntime::SetInput(backend.GetInput());
		scriptManager.Init();

		Entity cameraEntity = registry.CreateEntity("Camera");

		registry.AddComponent<ECS::TransformComponent>(cameraEntity, ECS::TransformComponent());
		float aspectRation = (float)initData.width / (float)initData.height;
		renderer.GetCamera()->AssignEntity(cameraEntity, &registry);
		renderer.GetCamera()->SetAspectRatio(aspectRation);

		backend.GetInput()->LockCursor(false);
	}

	void Engine::Shutdown() {
		renderer.Shutdown();
		backend.Shutdown();
	}

	void Engine::Run() {
		bool run = true;
		while (run) {
			time.Update();
			run = backend.Run();

			// Layers Update
			for (auto layer : layerStack.Layers()) {
				layer->OnUpdate();
			}
			for (auto layer : layerStack.Overlays()) {
				layer->OnUpdate();
			}

			UpdateSystems();

			renderer.InitRender(backend.GetCustomRenderTarget());
			renderer.Render();
			// Layers OnRender
			for (auto layer : layerStack.Layers()) {
				layer->OnRender();
			}
			for (auto layer : layerStack.Overlays()) {
				layer->OnRender();
			}
			renderer.EndRender();

			backend.GetRenderContext()->SetRenderTarget(backend.GetBackRenderTarget()->GetRTV(), nullptr);

			ImGuiLayer::NewFrame();
			// Layers OnRenderImGui
			for (auto layer : layerStack.Layers()) {
				layer->OnRenderImGui();
			}
			for (auto layer : layerStack.Overlays()) {
				layer->OnRenderImGui();
			}
			ImGuiLayer::EndFrame();

			backend.PresentFrame();
		}
	}

	void Engine::UpdateSystems() {
		scriptManager.Update();
		particleManager.Update(&registry);
		physicsManager.Update(&registry);
		renderer.GetCamera()->SetAspectRatio(ctx.sceneViewportSize.x / ctx.sceneViewportSize.y);
		renderer.GetCamera()->Update(&registry);
	}
}