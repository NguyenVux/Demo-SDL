#pragma once

class Time
{
public:
	static float GetDeltaTimeF() { return (float)deltaTime / 1000.f; }
	static uint64_t GetDeltaTime() { return deltaTime; }
	static void Loop();
private:
	static uint64_t lastFrameTicks;
	static uint64_t deltaTime;
};