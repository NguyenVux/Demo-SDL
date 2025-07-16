#pragma once
#include <cstdint>

class IState {
public:
	virtual void Enter() {}
	virtual void Exit() {}
	virtual void PreFrame() {}
	virtual void Update() {}
	virtual void Render() {}
	virtual void PostFrame() {}
	virtual ~IState() {}
};
