#include "pch.h"
#include "Animation.h"

Animation::Animation(SDL_Texture* texture) : 
	m_texture(texture),
	m_currentFrame(0), 
	m_accumulateTime(0), 
	m_fps(5), 
	m_isPlaying(false)
{

}

void Animation::Update() {
	if(!m_isPlaying) {
		return;
	}

	uint64_t delta = Time::GetDeltaTime();
	uint64_t frameTime = 1000 / m_fps;
	m_accumulateTime  += delta;
	if(m_accumulateTime > frameTime)
	{
		m_currentFrame = (m_currentFrame + m_accumulateTime / frameTime) % 1;
		m_accumulateTime = 0;
	}
}

void Animation::Render() {
	// Render the current frame
}

void Animation::Play() {
	m_isPlaying = true;
	m_currentFrame = 0;
}

void Animation::Stop() {
	m_isPlaying = false;
	m_currentFrame = 0;
}

void Animation::Pause() {
	m_isPlaying = false;
}

void Animation::Resume()
{
	m_isPlaying = true;
}
