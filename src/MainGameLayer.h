#pragma once
#include "ILayer.h"
#include "FSM.h"
#include "Animation.h"
class Application;
struct SDL_Texture;
class MainGameLayer : public ILayer
{
public:
	MainGameLayer(Application *app);
	~MainGameLayer() override;
	void PreFrame() override;
	void Update() override;
	void FlushCommandQueue() override;
	void PostFrame() override;
	void Destroy() override {}
	void Init() override;

private:
	FSM m_fsm;
	Application *m_app;
};
