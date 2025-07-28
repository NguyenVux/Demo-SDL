#pragma once
#include <SDL_rect.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdio.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <float.h>
#endif // __EMSCRIPTEN__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef NDEBUG
#define ASSERT(condition, message)                                             \
  if (!(condition))                                                            \
    fprintf(stderr, "%s\n", message);
#define ASSERT_M(message) ASSERT(false, message)
#else
#define ASSERT(condition, message)
#endif

#include "entt/entt.hpp"
#include "glm/glm.hpp"

#include "Animation.h"
#include "Application.h"
#include "AssetsManager.h"
#include "FSM.h"
#include "IState.h"
#include "Time.h"

enum LayerID { INVALID_ID, GAMELAYER_ID, DEBUG_ID, COUNT };
