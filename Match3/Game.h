
#ifndef _GAME_H_
#define _GAME_H_

#include "Block.h"

const int NUM_BLOCK_TYPES = 5;
const int NUM_BLOCK_COLUMNS = 8;
const int NUM_BLOCK_ROWS = 8;

class Game
{
	struct					impl;
	std::unique_ptr<impl>	pimpl;
public:
	Game(Renderer &r);
	~Game();

	void runEventLoop();
};

#endif