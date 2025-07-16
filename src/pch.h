#pragma once
#include <stdio.h>
#include <memory>
#ifdef EMSCRIPTEN
#include <SDL.h>
#include <emscripten.h>
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