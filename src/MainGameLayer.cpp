#include "pch.h"
#include "MainGameLayer.h"
#include <box2d/box2d.h>

b2Vec2 position;

MainGameLayer::MainGameLayer(Application *app) : 
	ILayer(LayerID::GAMELAYER_ID),
	m_app(app)
{

}

void MainGameLayer::PreFrame()
{
}
void MainGameLayer::Update()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_EventType::SDL_KEYDOWN:
	{
		if (event.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_UP)
		{
			position.y += 5;
		}
		if (event.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_DOWN)
		{
			position.y -= 5;
		}
		if (event.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_LEFT)
		{
			position.x -= 5;
		}
		if (event.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_RIGHT)
		{
			position.x += 5;
		}
	}
	break;

	default:
		break;
	}
}
void MainGameLayer::FlushCommandQueue()
{
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
void MainGameLayer::PostFrame()
{
}
MainGameLayer::~MainGameLayer()
{
}

void MainGameLayer::Init()
{

}
