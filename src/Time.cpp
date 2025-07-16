#include "pch.h"
#include "Time.h"

uint64_t Time::lastFrameTicks = 0;
uint64_t Time::deltaTime = 0;
void Time::Loop()
{
	uint64_t currentTick = SDL_GetTicks64();
	deltaTime = currentTick - lastFrameTicks;
	lastFrameTicks = currentTick;
}
