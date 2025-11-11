#ifndef APPLICATION_CLASS_H
#define APPLICATION_CLASS_H

#include"Window.h"
#include"Time.h"
#include"Renderer.h"

class Application {
public:
	Application(int width, int height, const char* title);
	~Application();

	void Run();

private:
	Input input;
	Window window;
	Time time;
	Renderer renderer;
};

#endif