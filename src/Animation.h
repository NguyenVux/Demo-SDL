#pragma once

struct Sprite;
struct SpriteSheet;

#include "pch.h"
#include <string>

class Animation
{
public:
	Animation();
	Animation(const std::string &name, int fps, SpriteSheet *sheet, bool isLoop);
	int GetFPS() const { return m_fps; }
	void SetFPS(int fps) { m_fps = fps; }
	uint64_t GetTimePerFrame(float speed) const;
	int GetLength() const;
	const std::string &GetName() const { return m_name; }
	const Sprite *GetSpriteAt(int frame) const;
	bool IsLoop() const { return m_isLoop; }

	void SetName(const std::string &name);
	void SetLoop(bool loop);
	void SetSheet(const SpriteSheet *sheet);

private:
	const SpriteSheet *m_sheet;
	int m_fps;
	std::string m_name;
	bool m_isLoop;
};

class AnimationInstance
{
public:
	AnimationInstance(const Animation* animation);
	void Play();
	void Stop();
	void Pause();
	void Resume();
	void Update();
	const Sprite *GetCurrentSprite() const;
	int m_speed;

private:
	const Animation* m_animation;
	int m_currentFrame;
	int m_accumulateTime;
	bool m_isPlaying;
};