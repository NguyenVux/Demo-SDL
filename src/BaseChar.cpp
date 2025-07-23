#include "pch.h"
#include "BaseChar.h"
#include "math.h"

BaseChar::BaseChar() : m_speed(200.0f),
					   m_scale(2.0f),
					   facingRight(true),
					   m_currentState(CharacterState::IDLE)
{
	m_normOrigin = {
		0.5f,
		0.0f};
	m_position = {
		0.0f,
		200.0f};
	m_velocity = {
		0.0f,
		10.0f};
}

void BaseChar::Update()
{

	CharacterState lastState = m_currentState;
	CharacterState nextState = m_currentState;

	constexpr float gravity = -9.8f * 44.4f;
	m_velocity.y += gravity * Time::GetDeltaTimeF();

	float x = m_velocity.x * Time::GetDeltaTimeF();
	float y = m_velocity.y * Time::GetDeltaTimeF();

	int length;
	const uint8_t *keys = SDL_GetKeyboardState(&length);
	if (abs(y) < FLT_EPSILON)
	{

		if (keys[SDL_SCANCODE_LEFT])
		{
			m_velocity.x = -m_speed;
			facingRight = false;
		}
		else if (keys[SDL_SCANCODE_RIGHT])
		{
			m_velocity.x = m_speed;
			facingRight = true;
		}
		else
		{
			m_velocity.x = 0.0f;
		}
	}

	if (keys[SDL_SCANCODE_X])
	{
		m_velocity.y += 10.f * 44.4f;
	}

	if (abs(m_velocity.x) < FLT_EPSILON)
	{
		m_currentState = CharacterState::IDLE;
	}
	else
	{
		m_currentState = CharacterState::MOVING;
	}
	if (y < 0.0f)
	{
		m_currentState = CharacterState::FALLING;
	}
	else if (y > 0.0f)
	{
		m_currentState = CharacterState::FALLING;
	}

	m_position.x += x;
	m_position.y += y;
	AnimationInstance *anim = GetCurrentAnimation();
	if (anim != nullptr)
	{
		if (lastState != m_currentState)
		{
			anim->Play();
		}
		anim->Update();
	}
}

AnimationInstance *BaseChar::GetCurrentAnimation()
{
	if (m_animations.find(m_currentState) != m_animations.end())
	{
		return m_animations[m_currentState].get();
	}
	return nullptr;
}

void BaseChar::Render(RenderQueue &queue)
{
	AnimationInstance *anim = GetCurrentAnimation();
	if (anim != nullptr)
	{
		RenderCommand command;
		command.Sprite = anim->GetCurrentSprite();
		float w = command.Sprite->SourceRect.w * m_scale;
		float h = command.Sprite->SourceRect.h * m_scale;
		float x = m_position.x - m_normOrigin.x * w;
		float y = 720.0f - m_position.y - m_normOrigin.y * h;
		command.DstRect = {x, y, w, h};
		command.Angle = 0.0f;
		command.Center = {x, y};
		command.Flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
		queue.Push(command);
	}
}

void BaseChar::RenderDebug(RenderQueue &queue)
{
}