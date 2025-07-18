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
	if (!app.IsInitialized()) {
		std::cout << "Initializing application..." << std::endl;
		app.Init();
		app.AddLayer(std::make_unique<MainGameLayer>(&app));
#ifndef NDEBUG
		app.AddLayer(std::make_unique<DebugLayer>());
#endif // NDEBUG
		std::cout << "Application initialized successfully" << std::endl;
	}
	Time::Loop();
	app.Loop();
}

int main(int argc, char* argv[]) {
	std::cout << "Starting application..." << std::endl;
	#ifdef __EMSCRIPTEN__
	std::cout << "Running in Emscripten mode" << std::endl;
	emscripten_set_main_loop(main_loop, 0, 0);
	#else // __EMSCRIPTEN__
	std::cout << "Running in native mode" << std::endl;
	while (true) {
		main_loop();
	}
	#endif // __EMSCRIPTEN__
	std::cout << "ABC" << std::endl;
	return 0;
}