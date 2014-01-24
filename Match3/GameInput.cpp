#include "GameImpl.h"
#include "SDL.h"

BlockPtr Game::impl::getSelectedBlock()
{
	return board->findBlock([] (BlockPtr b) -> bool {
		return b->isSelected();
	});
}


bool Game::impl::trySwap(const unsigned int currentTime, BlockPtr src, BlockPtr dst)
{
	if(!src->isNeighbor(dst))
	{
		return false;
	}
	if(!src->canMove())
	{
		return false;
	}
	if(!dst->canMove())
	{
		return false;
	}

	int srcX = src->getBoardX();
	int srcY = src->getBoardY();
	int dstX = dst->getBoardX();
	int dstY = dst->getBoardY();

	KillCalculator killCalculator(*board);
	killCalculator.swapTypes(srcX, srcY, dstX, dstY);
	killCalculator.calculateKills();
	if(!killCalculator.hasKills())
	{
		return false;
	}

	std::swap(board->blocks[dstY][dstX], board->blocks[srcY][srcX]);
	src->swapWith(currentTime, dst);

	return true;
}

void Game::impl::processMouseMotion(const unsigned int currentTime, int x, int y)
{
	if(!gameStarted)
	{
		return;
	}
	board->applyToAllBlocks([&] (BlockPtr b) {
		if(b->canMove() && b->isInside(x, y))
		{
			b->mark(currentTime);
		}
		else
		{
			b->unmark(currentTime);
		}
	});
}

void Game::impl::processMouseDown(const unsigned int currentTime, int x, int y)
{
	if(!gameStarted)
	{
		if(!tryGameStart(currentTime))
		{
			return;
		}
	}
	board->mouseDownBlock = board->findBlock([=] (BlockPtr b) -> bool {
		return b->canMove() && b->isInside(x, y);
	});
}

void Game::impl::processBlockClick(const unsigned int currentTime)
{
	BlockPtr selectedBlock = getSelectedBlock();
	if(!selectedBlock)
	{
		board->mouseDownBlock->select(currentTime);
		return;
	}
	if(board->mouseDownBlock == selectedBlock)
	{
		return;
	}
	//"select swap"
	if(!trySwap(currentTime, selectedBlock, board->mouseDownBlock))
	{
		selectedBlock->unselect(currentTime);
		board->mouseDownBlock->select(currentTime);
	}
}

void Game::impl::processBlockDrag(const unsigned int currentTime, int x, int y)
{
	//"drag swap"
	int dx, dy;
	board->mouseDownBlock->getSwapDirection(x, y, dx, dy);
	int srcX = board->mouseDownBlock->getBoardX();
	int srcY = board->mouseDownBlock->getBoardY();
	int dstX = srcX + dx;
	int dstY = srcY + dy;
	if(dstX >= 0 && dstX < NUM_BLOCK_COLUMNS &&
		dstY >= 0 && dstY < NUM_BLOCK_ROWS)
	{
		if(board->blocks[dstY][dstX])
		{
			trySwap(currentTime, board->mouseDownBlock, board->blocks[dstY][dstX]);
		}
	}
}

void Game::impl::processMouseUp(const unsigned int currentTime, int x, int y)
{
	if(!gameStarted)
	{
		return;
	}
	if(!board->mouseDownBlock)
	{
		return;
	}
	BlockPtr mouseUpBlock = board->findBlock([=] (BlockPtr b) -> bool {
		return b->canMove() && b->isInside(x, y);
	});

	if(mouseUpBlock == board->mouseDownBlock)
	{
		processBlockClick(currentTime);
	}
	else if(board->mouseDownBlock->canMove() && !board->mouseDownBlock->isInside(x, y))
	{
		processBlockDrag(currentTime, x, y);
	}
	board->mouseDownBlock = nullptr;
}

bool Game::impl::pollEvents(const unsigned int currentTime)
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_QUIT:
			//case SDL_KEYDOWN:
				return true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				processMouseDown(currentTime, e.motion.x, e.motion.y);
				break;
			case SDL_MOUSEMOTION:
				processMouseMotion(currentTime, e.motion.x, e.motion.y);
				break;
			case SDL_MOUSEBUTTONUP:
				processMouseUp(currentTime, e.motion.x, e.motion.y);
				break;
		}
	}
	return false;
}
