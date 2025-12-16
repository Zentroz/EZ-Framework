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
#include"Engine/Events/Event.h"
#include"Engine/Render/Frontend/EditorCamera.h"
#include"Engine/Project/Project.h"
#include"Engine/Scripting/Native/NativeScriptManager.h"

namespace ENGINE {

	class Engine {
	public:
		struct EngineInitData {
			int width; int height; const char* title;
		};

		struct EngineContext {
		public:
			Engine* GetEngine() { return engine; }
			ECS::Registry& Registry() { return engine->registry; }
			RENDERER::Renderer& Renderer() { return engine->renderer; }
			LayerStack& LayerStack() { return engine->layerStack; }
			ASSET::AssetManager& AssetManager() { return engine->assetManager; }
			RENDERER::ResourceManager& ResourceManager() { return engine->resourceManager; }
			RENDERER::EngineBackend& Backend() { return engine->backend; }
			EditorCamera& EditorCamera() { return engine->m_camera; }
			Ref<Scene> GetCurrentScene() { return engine->currentScene; }
			void LoadScene(std::string filepath) { engine->LoadScene(filepath); }
			Project& Project() { return engine->m_Project; }

			float2 sceneViewportSize = float2(1280, 720);

			void SetEngine(Engine* engine) { this->engine = engine; }

		private:
			Engine* engine = nullptr;
		};

		Engine();

		void Init(EngineInitData initData);
		void Shutdown();

		void OnEvent(Event& event);
		void Tick();

		void LoadScene(std::string filepath);

		void SaveProject();
		void LoadProject(std::string projectFilePath);
		void UpdateSystems();

		static EngineContext Get() { return ctx; }
		friend EngineContext;

	private:
		static EngineContext ctx;

		Project m_Project;
		EditorCamera m_camera;
		NativeScriptManger nativeScriptManager;

		GameTime time;
		ECS::Registry registry;
		RENDERER::Renderer renderer;
		RENDERER::EngineBackend backend;
		ASSET::AssetManager assetManager;
		RENDERER::ResourceManager resourceManager;
		LayerStack layerStack;
		SCRIPT::ScriptRuntime scriptManager;
		PHYSICS::PhysicsManager physicsManager;
		//ParticleManager particleManager;

		Scene currentScene;
	};

};