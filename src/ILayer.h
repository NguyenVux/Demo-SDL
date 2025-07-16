#pragma once

class ILayer {
public:
	virtual void Init() {}
	virtual void PreFrame() {}
	virtual void Update() {}
	virtual void Render() {}
	virtual void PostFrame() {}
	virtual void Destroy() {}
	virtual void Enable() {}
	virtual void Disable() {}
	virtual ~ILayer() {}
};
