#pragma once
#include "ILayer.h"
#include "FSM.h"
class Application;
class Animation;
class MainGameLayer : public ILayer {
public:
	MainGameLayer(Application* app);
	void PreFrame() override;
	void Update() override;
	void Render() override;
	void PostFrame() override;
	~MainGameLayer() override;
	void Destroy() override {}
	void Enable() override {}
	void Disable() override {}
	void Init() override;

private:
	FSM m_fsm;
	Animation* anim;
	Application* m_app;
};
