#pragma once
#include "entt/entity/fwd.hpp"
#include "entt/entt.hpp"
#include <memory>

class ISystem {
public:
  ISystem(entt::registry &registry);
  virtual void Update() = 0;
  virtual void Render() = 0;
  virtual void PostFrame() = 0;

protected:
  entt::registry &m_registry;
};

class Scene {
public:
  Scene();
  void Update();
  void Render();
  void PostFrame();

private:
  entt::registry m_registry;
  std::vector<std::unique_ptr<ISystem>> m_systems;
};
