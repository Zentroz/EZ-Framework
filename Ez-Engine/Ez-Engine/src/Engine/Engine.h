#pragma once

#include<functional>

#include"Engine/Core/GameTime.h"
#include"Engine/Core/Logger.h"

#include"Engine/Render/Backend/EngineBackend.h"
#include"Engine/Render/Frontend/Renderer.h"
#include"Engine/Render/Scene.h"
#include"Engine/Assets/AssetManager.h"
#include"Engine/Scripting/ScriptManager.h"
#include"Engine/Physics/PhysicsManager.h"
#include"Engine/ParticleManager/ParticleManager.h"
#include"Engine/ECS/Registry.h"
#include"Engine/Layer/LayerStack.h"
#include"Engine/Layer/ImGuiLayer.h"

namespace ENGINE {
	class Engine {
	public:
		struct EngineInitData {
			int width; int height; const char* title;
		};

		struct EngineContext {
		public:
			ECS::Registry* Registry() { return &engine->registry; }
			RENDERER::Renderer* Renderer() { return &engine->renderer; }
			LayerStack* LayerStack() { return &engine->layerStack; }
			ASSET::AssetManager* AssetManager() { return &engine->assetManager; }
			RENDERER::ResourceManager* ResourceManager() { return &engine->resourceManager; }
			RENDERER::EngineBackend* Backend() { return &engine->backend; }

			float2 sceneViewportSize = float2(1280, 720);

			void SetEngine(Engine* engine) { this->engine = engine; }

		private:
			Engine* engine;
		};

		Engine();

		void Init(EngineInitData initData);
		void Shutdown();
		void Run();

		void LoadScene(Scene* scene) {
			this->currentScene = scene;
			scene->SetRegistry(&registry);
			scene->Setup();
		}

		void UpdateSystems();

		static EngineContext Get() { return ctx; }
		friend EngineContext;

	private:
		static EngineContext ctx;

		GameTime time;
		ECS::Registry registry;
		RENDERER::Renderer renderer;
		RENDERER::EngineBackend backend;
		ASSET::AssetManager assetManager;
		RENDERER::ResourceManager resourceManager;
		LayerStack layerStack;
		SCRIPT::ScriptRuntime scriptManager;
		PHYSICS::PhysicsManager physicsManager;
		ParticleManager particleManager;

		Scene* currentScene = nullptr;
	};

};