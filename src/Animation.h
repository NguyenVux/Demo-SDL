#pragma once
#include <vector>

class SDL_Texture;

class Animation {
public:
	Animation(SDL_Texture* texture);
	void Play();
	void Stop();
	void Pause();
	void Update();
	void Render();

	SDL_Texture* m_texture; // Assuming you have a texture to render
	int m_currentFrame;
private:
	std::vector<int> m_frames;
	int m_accumulateTime;
	int m_fps;
	bool m_isPlaying;
};