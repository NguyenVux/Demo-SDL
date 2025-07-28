#include "MainGameLayer.h"
#include "pch.h"

MainGameLayer::MainGameLayer(Application *app)
    : ILayer(LayerID::GAMELAYER_ID), m_app(app) {}

void MainGameLayer::Update() {}
void MainGameLayer::FlushCommandQueue() {
  RenderCommand command;
  while (m_renderQueue.Dequeue(command)) {
    SDL_RenderCopyExF(m_app->GetRenderer(), command.Sprite->TexturePtr,
                      &command.Sprite->SourceRect, &command.DstRect,
                      command.Angle, &command.Center, command.Flip);
  }
}
void MainGameLayer::PostFrame() {}
MainGameLayer::~MainGameLayer() {}

void MainGameLayer::Init() {}
