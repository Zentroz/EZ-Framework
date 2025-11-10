#include"Time.h"

std::chrono::steady_clock::time_point Time::s_startTime;
std::chrono::steady_clock::time_point Time::s_lastFrameTime;

float Time::deltaTime = 0.0f;
float Time::time = 0.0f;
int Time::fps = 0;

float Time::fpsCounter;
float Time::fpsTimer;

void Time::Start() {
	// Initial Time Capture
	s_startTime = std::chrono::steady_clock::now();
	s_lastFrameTime = s_startTime;
}

void Time::Update() {
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