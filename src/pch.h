#pragma once
#include <stdio.h>
#include <memory>
#include <iostream>
#include <algorithm>
#ifdef __EMSCRIPTEN__ 
#include <emscripten.h>
#include <float.h>
#endif // __EMSCRIPTEN__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef NDEBUG
#define ASSERT(condition,message) if(!(condition)) fprintf(stderr, "%s\n", message);
#define ASSERT_M(message) ASSERT(false,message)
#else
#define ASSERT(condition,message)
#endif



#include "Time.h"
#include "Application.h"
#include "Animation.h"
#include "AssetsManager.h"
#include "FSM.h"
#include "IState.h"

enum LayerID
{
	INVALID_ID,
	GAMELAYER_ID,
	DEBUG_ID,
	COUNT		
};
