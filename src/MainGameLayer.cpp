#include "pch.h"
#include "MainGameLayer.h"
#include <box2d/box2d.h>


b2Vec2 position;

MainGameLayer::MainGameLayer(Application *app) : m_app (app)
{
}

void MainGameLayer::PreFrame()
{
}
void MainGameLayer::Update() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_EventType::SDL_KEYDOWN:
		{
			if(event.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_UP)
			{
				position.y += 5;
			}
			if(event.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_DOWN)
			{
				position.y -= 5;
			}
			if(event.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_LEFT)
			{
				position.x -= 5;
			}
			if(event.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_RIGHT)
			{
				position.x += 5;
			}
		}
		break;
	
	default:
		break;
	}

}
void MainGameLayer::Render() {
	SDL_SetRenderDrawColor(m_app->GetRenderer(), 255, 255, 255, 255);
	SDL_Rect boxRect = { static_cast<int>(position.x), static_cast<int>(position.y), 50, 50 };
	SDL_RenderFillRect(m_app->GetRenderer(), &boxRect);
}
void MainGameLayer::PostFrame() {

}
MainGameLayer::~MainGameLayer() {

}


void MainGameLayer::Init()
{

}
