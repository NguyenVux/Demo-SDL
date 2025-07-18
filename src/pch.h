#pragma once
#include <stdio.h>
#include <memory>
#ifdef __EMSCRIPTEN__ 
#include <emscripten.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#ifndef NDEBUG
#define ASSERT(condition,message) if(!(condition)) fprintf(stderr, "%s\n", message);
#define ASSERT(message) fprintf(stderr, "%s\n", message);
#else
#define ASSERT(condition,message)
#define ASSERT(condition)
#endif



#include "Time.h"
#include "Application.h"
#include "Animation.h"