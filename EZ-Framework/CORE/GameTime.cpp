#include"Core/GameTime.h"

std::chrono::steady_clock::time_point GameTime::s_startTime;
std::chrono::steady_clock::time_point GameTime::s_lastFrameTime;

float GameTime::deltaTime = 0.0f;
float GameTime::time = 0.0f;
int GameTime::fps = 0;

float GameTime::fpsCounter;
float GameTime::fpsTimer;

void GameTime::Start() {
	// Initial Time Capture
	s_startTime = std::chrono::steady_clock::now();
	s_lastFrameTime = s_startTime;
}

void GameTime::Update() {
	// Current Time Retrieval and Delta Time Calculation 
	
	auto currentTime = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration<float>(currentTime - s_lastFrameTime).count();
	s_lastFrameTime = currentTime;

	time = std::chrono::duration<float>(std::chrono::steady_clock::now() - s_startTime).count();

	// FPS Calculation
	fpsTimer += deltaTime;
	fpsCounter++;

	if (fpsTimer >= 1.0f) {
		fps = fpsCounter;
		fpsCounter = 0;
		fpsTimer = 0.0f;
	}
}