
#include "pch.h"
#include "Animation.h"

void AnimationInstance::Update()
{
	if (!m_isPlaying || m_animation->GetLength() < 1)
	{
		return;
	}
	uint64_t delta = Time::GetDeltaTime();
	uint64_t frameTime = m_animation->GetTimePerFrame(m_speed);
	m_accumulateTime += delta;
	if (m_accumulateTime > frameTime)
	{
		m_currentFrame = (m_currentFrame + m_accumulateTime / frameTime) % m_animation->GetLength();
		m_accumulateTime = 0;
	}
}

AnimationInstance::AnimationInstance(const Animation* animation) : 
	m_animation(std::move(animation)),
	m_currentFrame(0),
	m_accumulateTime(0),
	m_speed(1),
	m_isPlaying(0)
{
}

const Sprite *AnimationInstance::GetCurrentSprite() const
{
	if (m_animation == nullptr)
	{
		return nullptr;
	}
	return m_animation->GetSpriteAt(m_currentFrame);
}

void AnimationInstance::Play()
{
	m_isPlaying = true;
	m_currentFrame = 0;
}

void AnimationInstance::Stop()
{
	m_isPlaying = false;
	m_currentFrame = 0;
}

void AnimationInstance::Pause()
{
	m_isPlaying = false;
}

void AnimationInstance::Resume()
{
	m_isPlaying = true;
}


void Animation::SetName(const std::string& name) {
	m_name = name;
}

void Animation::SetLoop(bool loop) {
	m_isLoop = loop;
}

void Animation::SetSheet(const SpriteSheet* sheet) {
	m_sheet = sheet;
}

Animation::Animation():
	m_sheet(nullptr), 
	m_fps(0), 
	m_name(), 
	m_isLoop(false) 
{

}
Animation::Animation(const std::string &name, int fps, SpriteSheet *sheet, bool isLoop):
	m_name(name),
	m_fps(fps),
	m_sheet(sheet),
	m_isLoop(isLoop)
{
}

int Animation::GetLength() const { 
	return m_sheet ? static_cast<int>(m_sheet->Sprites.size()) : 0; 
}

uint64_t Animation::GetTimePerFrame(float speed) const
{
	if(abs(speed) < FLT_EPSILON)
	{
		return 1;
	}
	return abs(1000/(m_fps*speed));
}

const Sprite *Animation::GetSpriteAt(int frame) const
{
	if(m_sheet->Sprites.size() == 0)
	{
		return nullptr;
	}
	return m_sheet->Sprites[frame];
}
