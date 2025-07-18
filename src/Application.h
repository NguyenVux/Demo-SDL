#pragma once
#include <memory>
#include "ILayer.h"
#include "LayerStack.h"
#include "AssetsManager.h"

class SDL_Window;
class SDL_Renderer;
class FSM;

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
	AssetsManager& GetAssetsManager() const;
	const bool IsExiting() const { return m_isExiting; }
	void Exit() { m_isExiting = true; }
	void LoadAssets();
private:
	std::unique_ptr<FSM> m_fsm;
	LayerStack m_layerStack;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	std::unique_ptr<AssetsManager> m_assetManager;
	bool m_initialized;
	bool m_isExiting;
};
