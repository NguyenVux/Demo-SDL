
#pragma once
#include "pch.h"
#include "RenderQueue.h"

class ILayer
{
public:
	ILayer(int id) : m_id(id), m_enabled(true) {}
	virtual void Init() {}
	virtual void PreFrame() {}
	virtual void Update() {}
	virtual void FlushCommandQueue() {}
	virtual void PostFrame() {}
	virtual void Destroy() {}
	virtual void OnDisable() {};
	virtual void OnEnable() {};
	virtual void Enable() { 
		m_enabled = true; 
		m_renderQueue.Clear();
		OnEnable();
	}
	virtual void Disable() { 
		m_enabled = false; 
		OnDisable();
	}
	virtual bool IsEnabled() const { return m_enabled; }
	int GetID() { return m_id; }
	RenderQueue& GetRenderQueue() { return m_renderQueue; }
	virtual ~ILayer() {}

protected:
	bool m_enabled;
	int m_id;
	RenderQueue m_renderQueue;
};
