#pragma once
#include "ILayer.h"
#include <cstdio>

class DebugLayer : public ILayer {
public:
	void PreFrame() override;
	void Update() override;
	void Render() override;
	void PostFrame() override;
	~DebugLayer() override;
	void Destroy() override {}
	void Enable() override {}
	void Disable() override {}
};
