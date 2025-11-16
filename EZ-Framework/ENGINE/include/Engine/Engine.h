#pragma once

#include"Core/Time.h"
#include"Engine/EngineBackend.h"

#include"Renderer.h"
#include"Registry.h"
#include"ScriptManager.h"

class Engine {
public:
	Engine();

	void Init(int width, int height, const char* title);
	void Shutdown();
	void Frame();

private:
	EngineBackend backend;
	Renderer renderer;
	Time time;
};