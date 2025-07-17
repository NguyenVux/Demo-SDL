#include "pch.h"
#include "Animation.h"

Animation::Animation(SDL_Texture* texture) : 
	m_texture(texture),
	m_currentFrame(0), 
	m_accumulateTime(0), 
	m_fps(5), 
	m_isPlaying(false)
{
	m_frames.assign(1, 1);
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
		m_currentFrame = (m_currentFrame + m_accumulateTime / frameTime) % m_frames.size();
		m_accumulateTime = 0;
		// printf("Frame: %d\n", m_currentFrame);
	}
}

void Animation::Render() {
	// Render the current frame
}

void Animation::Play() {
	m_isPlaying = true;
}

void Animation::Stop() {
	m_isPlaying = true;
	m_currentFrame = 0;
}

void Animation::Pause() {
	m_isPlaying = false;
}
