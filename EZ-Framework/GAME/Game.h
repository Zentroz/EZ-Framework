#ifndef APPLICATION_CLASS_H
#define APPLICATION_CLASS_H

#include"Engine/Engine.h"
#include"TestScene.h"

class Game {
public:
	Game(int width, int height, const char* title);
	~Game();

	void Run();

private:
	Engine engine;
};

#endif