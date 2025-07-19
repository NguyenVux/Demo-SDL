#pragma once
#include "FSM.h"

class RenderQueue;
enum Direction {
	LEFT,
	RIGHT,
};


class CharacterRect {
};

class BaseChar {
public:
	BaseChar();
	Direction GetLookDirection() { return m_lookDirection; }
	void Render(const RenderQueue& queue);
	void RenderDebug(const RenderQueue& queue);
	void Move(Direction direction);
	SDL_FPoint GetPosition() { return m_position; }
private:
	float m_speed;
	SDL_FPoint m_position;
	SDL_FPoint m_normOrigin;
	Direction m_lookDirection;
};
