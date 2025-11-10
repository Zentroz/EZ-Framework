#ifndef TIME_CLASS_H 
#define TIME_CLASS_H
#include<chrono>

class Time {
public:
	static float deltaTime;
	static float time;
	static int fps;

	static void Start();
	static void Update();

private:
    static std::chrono::steady_clock::time_point s_startTime;
    static std::chrono::steady_clock::time_point s_lastFrameTime;

    static float fpsCounter;
    static float fpsTimer;
};

#endif