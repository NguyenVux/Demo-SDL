#pragma once
#include "FSM.h"

class RenderQueue;
enum Direction {
	LEFT,
	RIGHT,
};

enum CharacterState
{
	IDLE,
	MOVING,
	ATTACKING,
	DAMAGE,
};


class CharacterRect {
};


class BaseChar;

class BaseState : public IState {
protected:
	BaseState(BaseChar* owner, FSM* fsm): IState(fsm), m_owner(owner) {}
public:
	BaseChar* m_owner;
};

class Idle : public BaseState
{

};
class BaseChar {
public:
	BaseChar();
	Direction GetLookDirection() { return m_lookDirection; }
	void Render(const RenderQueue& queue);
	void RenderDebug(const RenderQueue& queue);
	void Move(Direction direction);
	void Idle();
	SDL_FPoint GetPosition() { return m_position; }
	std::unordered_map<CharacterState,AnimationInstance*> m_animations;
	// State logic
	void SetState(CharacterState state);
	void Update(float deltaTime);
private:
	float m_speed;
	float m_scale;
	SDL_FPoint m_position;
	SDL_FPoint m_normOrigin;
	Direction m_lookDirection;
	CharacterState m_currentState;
	FSM m_fsm;
};
