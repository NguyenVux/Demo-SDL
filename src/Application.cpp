#include "pch.h"
#include "Application.h"

Application::Application() : m_initialized(false), m_window(nullptr), m_renderer(nullptr) {}

void Application::AddLayer(std::unique_ptr<ILayer> layer) {
	layer->Init();
	m_layers.emplace_back(std::move(layer));
}

void Application::Init() {
	if (!m_initialized) {
		#ifdef EMSCRIPTEN
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			return;
		}
		m_window = SDL_CreateWindow("Demo-SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
		if (!m_window) {
			SDL_Quit();
			return;
		}
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!m_renderer) {
			SDL_DestroyWindow(m_window);
			m_window = nullptr;
			SDL_Quit();
			return;
		}
		#endif
		m_initialized = true;
	}
}

void Application::Destroy() {
	if (m_initialized) {
		#ifdef EMSCRIPTEN
		if (m_renderer) {
			SDL_DestroyRenderer(m_renderer);
			m_renderer = nullptr;
		}
		if (m_window) {
			SDL_DestroyWindow(m_window);
			m_window = nullptr;
		}
		SDL_Quit();
		#endif
		m_initialized = false;
	}
}

void Application::Loop() {
	if (m_initialized) {
		#ifdef EMSCRIPTEN
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_renderer);
		#endif
		for (auto& layer : m_layers) {
			layer->PreFrame();
			layer->Update();
			layer->Render();
		}
		#ifdef EMSCRIPTEN
		SDL_RenderPresent(m_renderer);
		#endif
		for (auto& layer : m_layers) {
			layer->PostFrame();
		}
	}
}

bool Application::IsInitialized() const { return m_initialized; }
SDL_Window* Application::GetWindow() const { return m_window; }
SDL_Renderer* Application::GetRenderer() const { return m_renderer; }
