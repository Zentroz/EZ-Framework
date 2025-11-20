#include"Game.h"

Game::Game(int width, int height, const char* title) : engine() {
	engine.Init(width, height, title);

	TestScene* test = new TestScene();

	engine.LoadScene(test);
}

Game::~Game() {
	engine.Shutdown();
}

void Game::Run() {
	engine.Frame();
}