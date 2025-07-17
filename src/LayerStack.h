#pragma once
#include <vector>
#include <memory>
#include "ILayer.h"

class LayerStack {
public:
	LayerStack();
	~LayerStack();
	
	void AddLayer(std::unique_ptr<ILayer> layer);
	void RemoveLayer(ILayer* layer);
	void Clear();
	
	void PreFrame();
	void Update();
	void Render();
	void PostFrame();
	
	size_t GetLayerCount() const;
	bool IsEmpty() const;

private:
	std::vector<std::unique_ptr<ILayer>> m_layers;
};
