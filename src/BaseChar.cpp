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

void BaseChar::Update() {

}

void BaseChar::Render(RenderQueue& queue)
{

}

void BaseChar::RenderDebug(RenderQueue& queue)
{
	
}