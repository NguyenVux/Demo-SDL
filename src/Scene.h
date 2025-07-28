#pragma once
#include "entt/entt.hpp"
#include <memory>

class ISystem {
public:
  ISystem(entt::registry &reg) : m_registry(reg) {}
  virtual void Update() {};
  virtual void Render() {};
  virtual void PostFrame() {};
  virtual ~ISystem() = default;

protected:
  entt::registry &m_registry;
};

class AnimationSystem : public ISystem{
public:
	AnimationSystem(entt::registry& reg) : ISystem(reg){}
	virtual ~AnimationSystem() override = default;
	virtual void Update() override;
private:

};

class RenderSystem : public ISystem {
public:
	RenderSystem(Application& app, entt::registry& reg) : ISystem(reg), m_app(app) {}
	virtual ~RenderSystem() override = default;
	virtual void Render() override;
private:
  Application& m_app;
};

class InputSystem : public ISystem {
public:
	InputSystem(entt::registry& reg) : ISystem(reg){}
	virtual ~InputSystem() override = default;
	virtual void Update() override;
private:
};

class IntegralMovementSystem : public ISystem {
public:
	IntegralMovementSystem(entt::registry& reg) : ISystem(reg){}
	virtual ~IntegralMovementSystem() override = default;
	virtual void Update() override;
private:
};

class CharStateSystem : public ISystem {
public:
	CharStateSystem(Application& app, entt::registry& reg) : ISystem(reg), m_app(app) {}
	virtual ~CharStateSystem() override = default;
	virtual void Update() override;
private:
  Application& m_app;
};



class Scene {
public:
  Scene();
  void Update();
  void Render();
  void PostFrame();
  std::vector<std::unique_ptr<ISystem>>& GetSystems() { return m_systems;}
  entt::registry& GetRegistry() { return m_registry;}

private:
  entt::registry m_registry;
  std::vector<std::unique_ptr<ISystem>> m_systems;
};
