#pragma once
#include "FSM.h"

class RenderQueue;

enum CharacterState
{
	IDLE,
	MOVING,
	ATTACKING,
	DAMAGE,
	FALLING,
	JUMPING,
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
	virtual void Update();
	AnimationInstance* GetCurrentAnimation();
	void Render(RenderQueue& queue);
	void RenderDebug(RenderQueue& queue);
	SDL_FPoint GetPosition() { return m_position; }
	std::unordered_map<CharacterState,std::unique_ptr<AnimationInstance>> m_animations;
private:
	SDL_FPoint m_velocity;
	float m_speed;
	float m_scale;
	SDL_FPoint m_position;
	SDL_FPoint m_normOrigin;
	CharacterState m_currentState;
	bool facingRight;
	FSM m_fsm;
};
