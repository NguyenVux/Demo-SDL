#pragma once

class SDL_Texture;

class Animation {
public:
	Animation(SDL_Texture* texture);
	Animation(const Animation&) = delete;
	Animation& operator=(const Animation&) = delete;
	Animation() = delete;
	void Play();
	void Stop();
	void Pause();
	void Resume();
	void Update();
	void Render();

	SDL_Texture* m_texture; // Assuming you have a texture to render
	int m_currentFrame;
private:
	int m_accumulateTime;
	int m_fps;
	bool m_isPlaying;
};