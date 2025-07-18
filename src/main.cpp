// #include "pch.h"
// #include <vector>
// #include <memory>

// #include "ILayer.h"
// #include "MainGameLayer.h"
// #include "DebugLayer.h"
// #include "Application.h"

// static Application app;

void main_loop()
{
// 	if (!app.IsInitialized()) {
// 		app.Init();
// 		app.AddLayer(std::make_unique<MainGameLayer>(&app));
// #ifndef NDEBUG
// 		app.AddLayer(std::make_unique<DebugLayer>());
// #endif // NDEBUG
// 	}
// 	Time::Loop();
// 	app.Loop();
}

int main() {
	#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop, 0, 0);
	#else // __EMSCRIPTEN__
	while (true) {
		main_loop();
	}
	#endif // __EMSCRIPTEN__
	return 0;
}