#pragma once
#include <vector>
#include <memory>
#ifdef EMSCRIPTEN
#include <SDL.h>
#endif
#include "ILayer.h"

class Application {
public:
	Application();
	void AddLayer(std::unique_ptr<ILayer> layer);
	void Init();
	void Destroy();
	void Loop();
	bool IsInitialized() const;
	SDL_Window* GetWindow() const;
	SDL_Renderer* GetRenderer() const;
private:
	std::vector<std::unique_ptr<ILayer>> m_layers;
	bool m_initialized;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
};
