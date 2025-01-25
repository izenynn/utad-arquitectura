#include "game/game.h"

//#include <SDL.h>
//#include <SDL_image.h>
//#include <SDL_ttf.h>
//#include <SDL_mixer.h>
//#include <glm/glm.hpp>
//#include <sol/sol.hpp>
//#include <imgui.h>

#include <iostream>

int main(int argc, char* argv[])
{
	(void)argc; (void)argv;
	std::cout << "Paco!" << std::endl;
	Game game;

	game.Initialize();
	game.Run();
	game.Destroy();

	return 0;
}
