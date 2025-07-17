#pragma once

class ILayer {
private:
	bool m_enabled = true;
public:
	virtual void Init() {}
	virtual void PreFrame() {}
	virtual void Update() {}
	virtual void Render() {}
	virtual void PostFrame() {}
	virtual void Destroy() {}
	virtual void Enable() { m_enabled = true; }
	virtual void Disable() { m_enabled = false; }
	virtual bool IsEnabled() const { return m_enabled; }
	virtual ~ILayer() {}
};
