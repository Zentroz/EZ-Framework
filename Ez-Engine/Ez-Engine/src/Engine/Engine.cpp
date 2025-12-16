#include"Engine.h"
#include"Engine/Serialization/Serializer.h"
#include"Engine/Serialization/BinarySerializer.h"
#include"Engine/Assets/MetaFileCache.h"

namespace ENGINE
{
	Engine::EngineContext Engine::ctx = {};

	Engine::Engine() : currentScene("New Scene") {
		currentScene.SetRegistry(&registry);
		Engine::ctx.SetEngine(this);
	}

	void Engine::Init(EngineInitData initData) {
		backend.Init({ initData.width, initData.height, initData.title });

		backend.CreateRenderTargetBackBuffer();
		backend.CreateRenderTargetTexture();

		ImGuiLayer::Setup();

		resourceManager.SetDevice(backend.GetDevice());
		resourceManager.SetAssetManager(&assetManager);

		renderer.Init({ backend.GetDevice(), backend.GetRenderContext(), &registry, &assetManager, &resourceManager, initData.width, initData.height });
		nativeScriptManager.Init();

		time.Start();

		SCRIPT::ScriptRuntime::SetRegistry(&registry);
		SCRIPT::ScriptRuntime::SetInput(backend.GetInput());
		scriptManager.Init();

		float aspectRation = (float)initData.width / (float)initData.height;
		m_camera.GetCamera().SetAspectRatio(aspectRation);
		renderer.SetCamera(&m_camera.GetCamera());

		backend.GetInput()->LockCursor(false);


		EUID e = currentScene.CreateEntity("TestEntity");
		registry.AddComponent(e, ECS::TransformComponent(float3::one(), float3::one(), quaternion::Identity()));
	}

	void Engine::Shutdown() {
		for (auto layer : layerStack.Layers()) layer->OnDetach();
		for (auto layer : layerStack.Overlays()) layer->OnDetach();

		ImGuiLayer::Destroy();
		renderer.Shutdown();
		backend.Shutdown();
	}

	void Engine::OnEvent(Event& event) {
		for (auto layer : layerStack.Layers()) layer->OnEvent(event);
		for (auto layer : layerStack.Overlays()) layer->OnEvent(event);

		m_camera.OnEvent(event);
	}

	void Engine::Tick() {
		// Get Window's messages
		backend.Run();

		// Update
		// Layers Update
		for (auto layer : layerStack.Layers()) {
			layer->OnUpdate();
		}
		for (auto layer : layerStack.Overlays()) {
			layer->OnUpdate();
		}

		UpdateSystems();

		// Render Scene
		renderer.InitRender(backend.GetCustomRenderTarget());
		renderer.Render();
		for (auto layer : layerStack.Layers()) {
			layer->OnRender();
		}
		for (auto layer : layerStack.Overlays()) {
			layer->OnRender();
		}
		renderer.EndRender();

		// Set Back buffer as RenderTarget
		backend.GetRenderContext()->SetRenderTarget(backend.GetBackRenderTarget()->GetRTV(), nullptr);

		// Render UI - ImGui
		ImGuiLayer::NewFrame();
		for (auto layer : layerStack.Layers()) {
			layer->OnRenderImGui();
		}
		for (auto layer : layerStack.Overlays()) {
			layer->OnRenderImGui();
		}
		ImGuiLayer::EndFrame();

		backend.PresentFrame();
	}

	void Engine::UpdateSystems() {
		time.Update();
		nativeScriptManager.Update();
		scriptManager.Update();
		//particleManager.Update(&registry);
		physicsManager.Update(&registry);
		m_camera.GetCamera().SetAspectRatio(ctx.sceneViewportSize.x / ctx.sceneViewportSize.y);
		m_camera.OnUpdate();
	}

	void Engine::LoadScene(std::string filepath) {
		Scene loadedScene("Loaded Scene");

		if (!Serializer::Deserialize(filepath.c_str(), loadedScene)) {
			LOG_ERROR("Failed to load scene");
			return;
		}

		registry.Reset();
		currentScene = loadedScene;
		currentScene.SetRegistry(&registry);
	}

	void Engine::SaveProject() {
		Serializer::SerializeProject(assetManager);
	}

	void Engine::LoadProject(std::string projectFilePath) {
		Serializer::DeserializeProject(projectFilePath, assetManager);
	}
}