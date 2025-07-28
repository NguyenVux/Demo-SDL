#pragma once

#include <cstdint>
namespace Components {

using Velocity = glm::vec2;
using Collider = SDL_FRect;

struct Transform
{
	glm::vec2 m_position;
	glm::vec2 m_scale;
};

struct AnimInstance {
  bool IsPlaying;
  float Speed;
  uint64_t CurrentFrame;
  uint64_t AccumulatedTime;
  const Animation* anim;
};

struct SpriteRenderData {
  const Sprite* spr;
  glm::vec2 m_origin;
};

struct CharState {
  bool IsFacingRight;
  bool lastStateMoving;
};

} // namespace Components

namespace Tags {

struct Player {};
struct Enemy {};

} // namespace Tags
