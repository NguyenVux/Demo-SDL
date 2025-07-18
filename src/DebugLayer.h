#pragma once
#include "ILayer.h"
#include <cstdio>

class Application;
class DebugLayer : public ILayer {
public:
	DebugLayer(Application* app);
	void PreFrame() override;
	void Update() override;
	void FlushCommandQueue() override;
	void PostFrame() override;
	~DebugLayer() override;
	void Destroy() override {}
private:
	Application* m_app;
};
