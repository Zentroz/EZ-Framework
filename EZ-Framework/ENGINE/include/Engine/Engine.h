#pragma once

#include"Core/GameTime.h"
#include"Engine/EngineBackend.h"
#include"Engine/Scene.h"

#include<functional>
#include"Renderer.h"
#include"ScriptManager.h"
#include"Registry.h"

class Engine {
public:
	Engine();

	void Init(int width, int height, const char* title);
	void Shutdown();
	void Frame();

	void LoadScene(Scene* scene) {
		this->currentScene = scene;
		scene->SetRegistry(&registry);
		scene->Setup();
	}

private:
	EngineBackend backend;
	Renderer renderer;
	GameTime time;
	ScriptManager scriptManager;
	Registry registry;

	Scene* currentScene = nullptr;
};