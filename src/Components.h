#pragma once

namespace Components {

using Position = glm::vec2;
using Velocity = glm::vec2;
using Collider = SDL_FRect;

}

namespace Tags {

struct Player {};
struct Enemy {};

}
