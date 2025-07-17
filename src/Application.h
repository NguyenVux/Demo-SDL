#pragma once
#include <memory>
#include "ILayer.h"
#include "LayerStack.h"

class SDL_Window;
class SDL_Renderer;

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
	LayerStack& GetLayerStack();
	const LayerStack& GetLayerStack() const;
private:
	LayerStack m_layerStack;
	bool m_initialized;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
};
