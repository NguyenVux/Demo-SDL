#include "pch.h"
#include "DebugLayer.h"

DebugLayer::DebugLayer(Application* app):
ILayer(LayerID::DEBUG_ID),
m_app(app)
{
}

void DebugLayer::PreFrame()
{
}
void DebugLayer::Update() {
}
void DebugLayer::FlushCommandQueue() {
	RenderCommand command;
	while (m_renderQueue.Dequeue(command))
	{
		SDL_RenderCopyExF(
			m_app->GetRenderer(),
			command.Sprite->TexturePtr,
			&command.Sprite->SourceRect,
			&command.DstRect,
			command.Angle,
			&command.Center,
			command.Flip
		);
	}
}
void DebugLayer::PostFrame() {
}
DebugLayer::~DebugLayer() {}
