#pragma once
#include "ILayer.h"
#include <memory>
#include <vector>

class LayerStack {
public:
  LayerStack();
  ~LayerStack();

  void AddLayer(std::unique_ptr<ILayer> layer);
  ILayer *GetLayerByID(int id);
  void RemoveLayer(ILayer *layer);
  void Clear();

  void Update();
  void FlushCommandQueue();
  void PostFrame();

  size_t GetLayerCount() const;
  bool IsEmpty() const;

private:
  std::vector<std::unique_ptr<ILayer>> m_layers;
};
