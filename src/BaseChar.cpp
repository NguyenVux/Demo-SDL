#include "pch.h"
#include "BaseChar.h"




BaseChar::BaseChar() :
	m_speed(20.0f),
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


void BaseChar::Render(const RenderQueue& queue)
{

}

void BaseChar::RenderDebug(const RenderQueue& queue)
{
	
}

void BaseChar::Move(Direction direction)
{
	if(direction != m_lookDirection)
	{
		m_lookDirection = direction;	
	}
	switch(direction)
	{
		case Direction::LEFT:
			{
				m_position.x -= m_speed;
			}
			break;
		case Direction::RIGHT:
			{
				m_position.x += m_speed;
			}
			break;
	}
}
