#include "pch.h"
#include "BaseChar.h"



BaseChar::BaseChar() :
	m_speed(2.5f),
	m_scale(1.0f),
	m_currentState(CharacterState::IDLE),
	m_lookDirection(Direction::RIGHT)
{
	m_normOrigin = {
		0.5f,
		0.0f
	};
	m_position = {
		0.0f,
		0.0f
	};
}


void BaseChar::SetState(CharacterState state) {
	if (m_currentState == state) return;

}

void BaseChar::Update(float deltaTime) {

}

void BaseChar::Render(RenderQueue& queue)
{

}

void BaseChar::RenderDebug(RenderQueue& queue)
{
	
}