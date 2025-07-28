#include "Scene.h"
#include "pch.h"
#include <glm/fwd.hpp>
#include "Components.h"
Scene::Scene() {}

void Scene::Update()
{
	for (auto &i : m_systems)
	{
		ASSERT(i != nullptr, "system ptr is null");
		if (i == nullptr)
		{
			continue;
		}
		i->Update();
	}
}
void Scene::Render()
{
	for (auto &i : m_systems)
	{
		ASSERT(i != nullptr, "system ptr is null");
		if (i == nullptr)
		{
			continue;
		}
		i->Render();
	}
}
void Scene::PostFrame()
{
	for (auto &i : m_systems)
	{
		ASSERT(i != nullptr, "system ptr is null");
		if (i == nullptr)
		{
			continue;
		}
		i->PostFrame();
	}
}

void AnimationSystem::Update()
{

	auto view = m_registry.view<Components::AnimInstance, Components::SpriteRenderData>();
	uint64_t deltaTime = Time::GetDeltaTime();
	for (auto [enitty, animInstance, renderData] : view.each())
	{

		auto anim = animInstance.anim;
		if (animInstance.IsPlaying)
		{
			uint64_t SecondPerFrame = 1000 / (anim->GetFPS() * animInstance.Speed);
			animInstance.AccumulatedTime += deltaTime;
			if (animInstance.AccumulatedTime > SecondPerFrame)
			{
				animInstance.AccumulatedTime -= SecondPerFrame;
				animInstance.CurrentFrame =
					(animInstance.CurrentFrame + 1) % anim->GetLength();
			}
		}

		renderData.spr = anim->GetSpriteAt(animInstance.CurrentFrame);
	}
}

void RenderSystem::Render()
{
	ILayer *layer = m_app.GetLayerStack().GetLayerByID(LayerID::GAMELAYER_ID);
	if (layer)
	{
		auto view = m_registry.view<Components::Transform, Components::CharState, Components::SpriteRenderData>();
		// const Sprite* spr = m_app->GetAssetsManager().GetSprite("idle_1");
		for (auto [entity, transform, state, renderData] : view.each())
		{
			const Sprite *spr = renderData.spr;
			glm::vec2 pos = transform.m_position;
			RenderCommand command{
				.Sprite = spr,
				.DstRect = {pos.x, pos.y, (float)spr->SourceRect.w,
							(float)spr->SourceRect.h},
				.Angle = 0.0f,
				.Center = {0.0f, 0.0f},
				.Flip = state.IsFacingRight ? SDL_RendererFlip::SDL_FLIP_NONE : SDL_RendererFlip::SDL_FLIP_HORIZONTAL,

			};
			layer->GetRenderQueue().Push(command);
		}
	}
}

void InputSystem::Update()
{
	int numKeys;
	const uint8_t *arr = SDL_GetKeyboardState(&numKeys);

	auto view = m_registry.view<Components::Velocity, Components::CharState, Tags::Player>();

	for (auto [entity, vel, state] : view.each())
	{
		if (arr[SDL_Scancode::SDL_SCANCODE_LEFT])
		{
			vel.x = -10.0f;
			state.IsFacingRight = false;
		}
		else if (arr[SDL_Scancode::SDL_SCANCODE_RIGHT])
		{
			vel.x = 10.0f;
			state.IsFacingRight = true;
		}
		else
		{
			vel.x = 0.0f;
		}
	}
}

void IntegralMovementSystem::Update()
{
	auto view = m_registry.view<Components::Velocity, Components::Transform>();

	for (auto [entity, vel, transform] : view.each())
	{
		transform.m_position += vel;
	}
}

void CharStateSystem::Update()
{
	auto view = m_registry.view<Components::Velocity, Components::CharState>();

	for (auto [entity, vel, state] : view.each())
	{
		if (m_registry.any_of<Components::AnimInstance>(entity))
		{
			auto& anim = m_registry.get<Components::AnimInstance>(entity);
			if(abs(vel.x) > FLT_EPSILON && !state.lastStateMoving)
			{
				anim.anim = m_app.GetAssetsManager().GetAnimation("run");
				anim.AccumulatedTime = 0;
				anim.CurrentFrame = 0;
				state.lastStateMoving = true;
			}
			else if(abs(vel.x) < FLT_EPSILON && state.lastStateMoving)
			{
				anim.anim = m_app.GetAssetsManager().GetAnimation("idle");
				anim.AccumulatedTime = 0;
				anim.CurrentFrame = 0;
				state.lastStateMoving = false;
			}
		}
	}
}
