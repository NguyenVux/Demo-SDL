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

void LayerStack::Render() {
	for (auto& layer : m_layers) {
		if (layer != nullptr) {
			layer->Render();
		}
	}
}

void LayerStack::PostFrame() {
	for (auto& layer : m_layers) {
		if (layer != nullptr) {
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
