
#include "Application.h"
#include "Components.h"
#include "RenderQueue.h"
#include "entt/entity/fwd.hpp"
#include "pch.h"
#include <SDL_render.h>
#include <cstdint>
#include <format>
enum class GameState : int { GAMEPLAY_STATE = 0 };

class DemoGameState : public IState {
private:
  entt::registry registry;
  entt::entity entity;
  const Animation *anim;

public:
  DemoGameState(Application *app) : IState(app->m_fsm.get()), m_app(app) {}
  ~DemoGameState() override = default;

  void Enter() override {
    anim = m_app->GetAssetsManager().GetAnimation("run");
    using namespace Components;
    entity = registry.create();
    registry.emplace<Position>(entity, 0.0f, 0.0f);
    registry.emplace<Collider>(entity, 0.0f, 0.0f, 120.0f, 80.0f);
    registry.emplace<AnimInstance>(entity, anim->GetLength(), 0ULL, 3.0f, 0ULL,
                                   anim->GetFPS());

    entity = registry.create();
    registry.emplace<Position>(entity, 0.0f, 80.0f);
    registry.emplace<Collider>(entity, 0.0f, 0.0f, 120.0f, 80.0f);
    registry.emplace<AnimInstance>(entity, anim->GetLength(), 0ULL, 1.0f, 0ULL,
                                   anim->GetFPS());
  }

  void Exit() override {
    // TODO: Add logic for exiting the demo game state
    m_animationInstance.reset();
  }

  void Update() override {
    auto view = registry.view<Components::AnimInstance>();
    uint64_t deltaTime = Time::GetDeltaTime();
    for (auto [enitty, animInstance] : view.each()) {
      uint64_t SecondPerFrame = 1000 / (animInstance.FPS * animInstance.Speed);
      animInstance.AccumulatedTime += deltaTime;
      if (animInstance.AccumulatedTime > SecondPerFrame) {
        animInstance.AccumulatedTime -= SecondPerFrame;
        animInstance.CurrentFrame =
            (animInstance.CurrentFrame + 1) % animInstance.FrameCount;
      }
    }
  }

  void Render() override {
    ILayer *layer = m_app->GetLayerStack().GetLayerByID(LayerID::GAMELAYER_ID);
    if (layer) {
      auto view = registry.view<Components::Position, Components::Collider,
                                Components::AnimInstance>();
      // const Sprite* spr = m_app->GetAssetsManager().GetSprite("idle_1");
      for (auto [entity, pos, collider, animInstance] : view.each()) {
        const Sprite *spr = anim->GetSpriteAt(animInstance.CurrentFrame);
        RenderCommand command{
            .Sprite = spr,
            .DstRect = {pos.x, pos.y, (float)spr->SourceRect.w,
                        (float)spr->SourceRect.h},
            .Angle = 0.0f,
            .Center = {0.0f, 0.0f},
            .Flip = SDL_RendererFlip::SDL_FLIP_NONE,

        };
        layer->GetRenderQueue().Push(command);
      }
    }
    layer = m_app->GetLayerStack().GetLayerByID(LayerID::DEBUG_ID);
    if (layer) {
    }
  }

  void PostFrame() override {}

private:
  std::unique_ptr<AnimationInstance> m_animationInstance;
  Application *m_app;
};

Application::Application()
    : m_initialized(false), m_window(nullptr), m_renderer(nullptr),
      m_isExiting(false), m_fsm(std::make_unique<FSM>()),
      m_assetManager(nullptr) {

  m_fsm->AddState(static_cast<int>(GameState::GAMEPLAY_STATE),
                  std::make_unique<DemoGameState>(this));
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
    m_window =
        SDL_CreateWindow("Demo-SDL", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
      printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
      Exit();
      return;
    }
    std::cout << "Creating renderer..." << std::endl;
    m_renderer = SDL_CreateRenderer(
        m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

    // Initialize AssetsManager after renderer is created
    m_assetManager = std::make_unique<AssetsManager>(m_renderer);

    LoadAssets();
    printf("Application initialized successfully.\n");
    m_initialized = true;
    // Now safe to request state change
    m_fsm->RequestStateChange(static_cast<int>(GameState::GAMEPLAY_STATE));
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
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        m_initialized = false;
        Exit();
        return;
      }
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_C) {
          ILayer *layer = m_layerStack.GetLayerByID(DEBUG_ID);
          if (layer != nullptr) {
            if (layer->IsEnabled()) {
              layer->Disable();
            } else {
              layer->Enable();
            }
          }
        }
      }
    }
    // Call FSM lifecycle
    m_fsm->Update();
    m_layerStack.Update();

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    m_fsm->Render();
    m_layerStack.FlushCommandQueue();
    SDL_RenderPresent(m_renderer);

    m_fsm->PostFrame();
    m_layerStack.PostFrame();
  }
}

bool Application::IsInitialized() const { return m_initialized; }
SDL_Window *Application::GetWindow() const { return m_window; }
SDL_Renderer *Application::GetRenderer() const { return m_renderer; }

LayerStack &Application::GetLayerStack() { return m_layerStack; }

const LayerStack &Application::GetLayerStack() const { return m_layerStack; }

AssetsManager &Application::GetAssetsManager() const {
  // Assert that the assets manager is initialized
  if (!m_assetManager) {
    throw std::runtime_error(
        "AssetsManager not initialized. Call Init() first.");
  }
  return *m_assetManager;
}

void Application::LoadAssets() {
  m_assetManager->LoadTexture("assets/player/_AttackCombo.png");
  m_assetManager->LoadTexture("assets/player/_Idle.png");
  m_assetManager->LoadTexture("assets/player/_Run.png");

  std::vector<std::string> sprites;

  constexpr int SpriteW = 120;
  constexpr int SpriteH = 80;
  for (int row = 0; row < 1; row++) {
    for (int col = 0; col < 10; col++) {
      sprites.emplace_back(std::format("idle_{}", sprites.size()));

      // Last element is newly added name
      SDL_Rect rect;
      rect.x = col * SpriteW;
      rect.y = row * SpriteH;
      rect.w = SpriteW;
      rect.h = SpriteH;
      m_assetManager->CreateSprite((*sprites.crbegin()),
                                   "assets/player/_Idle.png", rect);
    }
  }
  m_assetManager->CreateSpriteSheet("idle", sprites);
  sprites.clear();
  for (int row = 0; row < 1; row++) {
    for (int col = 0; col < 10; col++) {
      sprites.emplace_back(std::format("attack_{}", sprites.size()));

      // Last element is newly added name
      SDL_Rect rect;
      rect.x = col * SpriteW;
      rect.y = row * SpriteH;
      rect.w = SpriteW;
      rect.h = SpriteH;
      m_assetManager->CreateSprite((*sprites.crbegin()),
                                   "assets/player/_AttackCombo.png", rect);
    }
  }
  m_assetManager->CreateSpriteSheet("attack", sprites);
  sprites.clear();
  for (int row = 0; row < 1; row++) {
    for (int col = 0; col < 10; col++) {
      sprites.emplace_back(std::format("run_{}", sprites.size()));

      // Last element is newly added name
      SDL_Rect rect;
      rect.x = col * SpriteW;
      rect.y = row * SpriteH;
      rect.w = SpriteW;
      rect.h = SpriteH;
      m_assetManager->CreateSprite((*sprites.crbegin()),
                                   "assets/player/_Run.png", rect);
    }
  }
  m_assetManager->CreateSpriteSheet("run", sprites);

  m_assetManager->CreateAnimation("idle", "idle", 10, true);
  m_assetManager->CreateAnimation("attack", "attack", 10, false);
  m_assetManager->CreateAnimation("run", "run", 10, false);
}
