#include <iostream>
#include <SDL.h>
#include <math.h>

#include "Game.h"

int main(int argc, char **argv)
{
	try
	{
		SDLRenderer ren;
		Game game(ren);

		game.runEventLoop();
	}
	catch(RendererException &re)
	{
		std::cout << re.error << std::endl;
		return 1;
	}

	return 0;
}
