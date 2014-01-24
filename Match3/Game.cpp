#include "GameImpl.h"
#include "SDL.h"
#include <sstream>

Game::impl::impl(Renderer &r) :
renderer(r),
board(new Board(r)),
gameStarted(false),
gameStartTime(0),
gameStopTime(0),
timeLeftSeconds(TIME_LIMIT),
score(0),
firstGame(true)
{
}

Game::impl::~impl()
{
}

void Game::impl::render(const unsigned int currentTime)
{
	renderer.clear();
	renderer.drawBackground(TID_BACKGROUND);

	renderer.setClipRect(BOARD_POS_X, BOARD_POS_Y, NUM_BLOCK_COLUMNS * BLOCK_SIZE_X, NUM_BLOCK_ROWS * BLOCK_SIZE_Y);

	board->applyToAllBlocks([&] (BlockPtr b) {
		b->simulate(currentTime);
		b->render(currentTime);
	});


	board->applyToAllBlocks([&] (BlockPtr b) {
		b->renderOverlay(currentTime);
	});

	renderer.resetClipRect();

	std::ostringstream timeStream;
	timeStream << "Time: "  << timeLeftSeconds;
	renderer.drawText(timeStream.str().c_str(), 25, 125);

	std::ostringstream scoreStream;
	scoreStream << "Score: "  << score;
	renderer.drawText(scoreStream.str().c_str(), 25, 175);

	renderer.present();
}

void Game::impl::runEventLoop()
{
	board->generate();

	bool quit = false;

	while(!quit)
	{
		unsigned int currentTime = SDL_GetTicks();

		if(pollEvents(currentTime))
		{
			quit = true;
		}

		simulate(currentTime);

		render(currentTime);
	}
}

Game::Game(Renderer &r)
{
	pimpl = std::unique_ptr<impl>(new impl(r));
}

Game::~Game()
{
}

void Game::runEventLoop()
{
	pimpl->runEventLoop();
}
