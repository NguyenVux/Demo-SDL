#include "pch.h"
#include "Application.h"

Application::Application()
	: m_initialized(false)
	, m_window(nullptr)
	, m_renderer(nullptr)
	, m_isExiting(false) {
}

void Application::AddLayer(std::unique_ptr<ILayer> layer) {
	m_layerStack.AddLayer(std::move(layer));
}

void Application::Init() {
	   std::cout << "Application::Init() called" << std::endl;
	   if (!m_initialized) {
			   std::cout << "Initializing SDL..." << std::endl;
			   int initFlags = SDL_INIT_VIDEO;
			   if (SDL_Init(initFlags) != 0) {
					   printf("SDL_Init failed: %s\n", SDL_GetError());
					   Exit();
					   return;
			   }
			   std::cout << "Creating window..." << std::endl;
			   m_window = SDL_CreateWindow("Demo-SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
			   if (m_window == nullptr) {
					   printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
					   Exit();
					   return;
			   }
			   std::cout << "Creating renderer..." << std::endl;
			   m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			   if (m_renderer == nullptr) {
					   printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
					   Exit();
					   return;
			   }
			   int imgFlags = IMG_INIT_PNG;
			   if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
					   printf("SDL_image initialization failed: %s\n", IMG_GetError());
					   Exit();
					   return;
			   }
			   printf("SDL_image initialized successfully.\n");
			   printf("Application initialized successfully.\n");
			   m_initialized = true;
	   }
}

void Application::Destroy() {
	   if (m_renderer != nullptr) {
			   SDL_DestroyRenderer(m_renderer);
			   m_renderer = nullptr;
	   }
	   if (m_window != nullptr) {
			   SDL_DestroyWindow(m_window);
			   m_window = nullptr;
	   }
	   SDL_Quit();
	   m_initialized = false;
}

void Application::Loop() {
	if (m_initialized) {
		// Handle SDL events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				m_initialized = false;
				Exit();
				return;
			}
		}
		
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_renderer);
		
		m_layerStack.PreFrame();
		m_layerStack.Update();
		m_layerStack.Render();
		
		SDL_RenderPresent(m_renderer);
		
		m_layerStack.PostFrame();
	}
}

bool Application::IsInitialized() const { return m_initialized; }
SDL_Window* Application::GetWindow() const { return m_window; }
SDL_Renderer* Application::GetRenderer() const { return m_renderer; }

LayerStack& Application::GetLayerStack() {
	return m_layerStack;
}

const LayerStack& Application::GetLayerStack() const {
	return m_layerStack;
}
