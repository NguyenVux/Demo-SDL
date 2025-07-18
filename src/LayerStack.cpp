#include "pch.h"
#include "LayerStack.h"

LayerStack::LayerStack() {
}

LayerStack::~LayerStack() {
	Clear();
}

void LayerStack::AddLayer(std::unique_ptr<ILayer> layer) {
	if (layer != nullptr) {
		layer->Init();
		m_layers.emplace_back(std::move(layer));
	}
}

ILayer* LayerStack::GetLayerByID(int id)
{
	for(auto& layer : m_layers)
	{
		if(layer == nullptr || layer->GetID() != id)
		{
			continue;
		}
		return layer.get();
	}
	return nullptr;
}

void LayerStack::RemoveLayer(ILayer* layer) {
	if (layer == nullptr) {
		return;
	}
	
	auto iter = std::find_if(m_layers.begin(), m_layers.end(),
		[layer](const std::unique_ptr<ILayer>& layerPtr) {
			return layerPtr.get() == layer;
		});
	
	if (iter != m_layers.end()) {
		(*iter)->Destroy();
		m_layers.erase(iter);
	}
}

void LayerStack::Clear() {
	for (auto& layer : m_layers) {
		if (layer != nullptr) {
			layer->Destroy();
		}
	}
	m_layers.clear();
}

void LayerStack::PreFrame() {
	for (auto& layer : m_layers) {
		if (layer != nullptr) {
			if(!layer->IsEnabled()) {
				continue;
			}
			layer->PreFrame();
		}
	}
}

void LayerStack::Update() {
	for (auto& layer : m_layers) {
		if (layer != nullptr) {
			if(!layer->IsEnabled()) {
				continue;
			}
			layer->Update();
		}
	}
}

void LayerStack::FlushCommandQueue() {
	for (auto& layer : m_layers) {
		if (layer != nullptr) {
			if(!layer->IsEnabled()) {
				layer->GetRenderQueue().Clear();
				continue;
			}
			layer->FlushCommandQueue();
		}
	}
}

void LayerStack::PostFrame() {
	for (auto& layer : m_layers) {
		if (layer != nullptr) {
			if(!layer->IsEnabled()) {
				continue;
			}
			layer->PostFrame();
		}
	}
}

size_t LayerStack::GetLayerCount() const {
	return m_layers.size();
}

bool LayerStack::IsEmpty() const {
	return m_layers.empty();
}
