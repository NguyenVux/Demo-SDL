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
		app.Init();
		app.AddLayer(std::make_unique<MainGameLayer>(&app));
#ifndef NDEBUG
		app.AddLayer(std::make_unique<DebugLayer>());
#endif // NDEBUG
	}
	Time::Loop();
	app.Loop();
}

int main() {
	Time::Loop();
	FILE *file = fopen("assets/.gitattributes", "rw");
	// Print all contents of the file
	if (file) {
		char buffer[256];
		while (fgets(buffer, sizeof(buffer), file)) {
			printf("%s", buffer);
		}
		fclose(file);
	}
	emscripten_set_main_loop(main_loop, 0, 0);
	return 0;
}