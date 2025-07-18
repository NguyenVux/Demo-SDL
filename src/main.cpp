#include "pch.h"
#include <vector>
#include <memory>

#include "ILayer.h"
#include "MainGameLayer.h"
#include "DebugLayer.h"
#include "Application.h"

static Application app;

void main_loop()
{
	if(app.IsExiting())
	{
#ifdef __EMSCRIPTEN__
		app.Destroy();
		emscripten_cancel_main_loop();
		return;
#endif // __EMSCRIPTEN__	
	}
	Time::Loop();
	app.Loop();
}

void init()
{
	std::cout << "Running in native mode" << std::endl;
	app.Init();
	std::cout << "Initializing application..." << std::endl;
	app.AddLayer(std::make_unique<MainGameLayer>(&app));
#ifndef NDEBUG
	app.AddLayer(std::make_unique<DebugLayer>(&app));
#endif // NDEBUG
}

int main(int argc, char *argv[])
{
	std::cout << "Starting application..." << std::endl;
#ifdef __EMSCRIPTEN__
	std::cout << "Running in Emscripten mode" << std::endl;
	emscripten_set_main_loop(main_loop, 0, 0);
	emscripten_pause_main_loop();
	init();
	emscripten_resume_main_loop();
#else // __EMSCRIPTEN__
	init();
	std::cout << "Application initialized successfully" << std::endl;
	while (!app.IsExiting())
	{
		main_loop();
	}
#endif // __EMSCRIPTEN__
	return 0;
}