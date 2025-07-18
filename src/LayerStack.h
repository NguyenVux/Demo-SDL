#pragma once
#include <vector>
#include <memory>
#include "ILayer.h"

class LayerStack {
public:
	LayerStack();
	~LayerStack();
	
	void AddLayer(std::unique_ptr<ILayer> layer);
	ILayer* GetLayerByID(int id);
	void RemoveLayer(ILayer* layer);
	void Clear();
	
	void PreFrame();
	void Update();
	void FlushCommandQueue();
	void PostFrame();

	size_t GetLayerCount() const;
	bool IsEmpty() const;

private:
	std::vector<std::unique_ptr<ILayer>> m_layers;
};
