#include "engine/engine.h"

int main(int argc, char* argv[])
{
	(void)argc; (void)argv;

	Engine engine;

	engine.Init();
	engine.Run();
	engine.Quit();

	return 0;
}