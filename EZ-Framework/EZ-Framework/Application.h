#ifndef APPLICATION_CLASS_H
#define APPLICATION_CLASS_H

#include"Window.h"
#include"Time.h"

class Application {
public:
	Application(int width, int height, const char* title);
	~Application();

	void Run();

private:
	Window window;
	Time time;
};

#endif