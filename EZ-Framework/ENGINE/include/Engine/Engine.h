#pragma once

#include"Core/Time.h"

#include"Window.h"
#include"Time.h"
#include"Renderer.h"
#include"Registry.h"
#include"ScriptManager.h"

class Engine {
public:
	Engine() = default;

	void Init(int width, int height, const char* title);
	void Shutdown();
	void Frame();

private:
	Input input;
	Window window;
	Time time;
	Renderer renderer;
	Registry registry;
	ScriptManager scriptManager;
};