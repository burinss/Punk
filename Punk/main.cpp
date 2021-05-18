
#include "PEngine.hpp"

int main() {
	setlocale(LC_ALL, "Russian");
	PSystemVariables settings;
	settings.windowSettings.windowSize = glm::vec2(1366, 768);
	settings.windowSettings.maximized = false;
	PunkBase engine;
	engine.Initialize(settings);
	engine.Run();
	return 0;
}