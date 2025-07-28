#pragma once

#include <cstdint>
namespace Components {

using Position = glm::vec2;
using Velocity = glm::vec2;
using Collider = SDL_FRect;

struct AnimInstance {
  int FrameCount;
  uint64_t CurrentFrame;
  float Speed;
  uint64_t AccumulatedTime;
  int FPS;
};

} // namespace Components

namespace Tags {

struct Player {};
struct Enemy {};

} // namespace Tags
