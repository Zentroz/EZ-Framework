#ifndef APPLICATION_CLASS_H
#define APPLICATION_CLASS_H

#include"Engine/Engine.h"

class Application {
public:
	Application(int width, int height, const char* title);
	~Application();

	void Run();

private:
	Engine engine;
};

#endif