#include "GameImpl.h"
#include <ctime>
#include <memory>

Board::Board(Renderer &r) :
renderer(r)
{
	rng.seed((unsigned int)std::time(0));
}

void Board::applyToAllBlocks(const std::function<void (BlockPtr&)> &f)
{
	for(int i = 0; i < NUM_BLOCK_ROWS; ++i)
	{
		for(int j = 0; j < NUM_BLOCK_COLUMNS; ++j)
		{
			if(!blocks[i][j])
			{
				continue;
			}
			f(blocks[i][j]);
		}
	}
}

BlockPtr Board::findBlock(const std::function<bool (BlockPtr)> &predicate)
{
	for(int i = 0; i < NUM_BLOCK_ROWS; ++i)
	{
		for(int j = 0; j < NUM_BLOCK_COLUMNS; ++j)
		{
			if(!blocks[i][j])
			{
				continue;
			}
			if(predicate(blocks[i][j]))
			{
				return blocks[i][j];
			}
		}
	}
	return nullptr;
}

void Board::generate()
{
	std::uniform_int<>		block_dist(TID_BLOCK_1, TID_BLOCK_1 + NUM_BLOCK_TYPES - 1);

	for(int i = 0; i < NUM_BLOCK_ROWS; ++i)
	{
		for(int j = 0; j < NUM_BLOCK_COLUMNS; ++j)
		{
			BlockPtr block = std::make_shared<Block>(renderer);
			block->init(j, i, (TextureID)block_dist(rng)); 
			blocks[i][j] = block;
		}
	}
}

int Board::simulateKills(const unsigned int currentTime)
{
	KillCalculator killCalculator(*this);
	killCalculator.calculateKills();

	int killCount = 0;
	applyToAllBlocks([&] (BlockPtr b) {
		if(killCalculator.hasKillAt(b->getBoardX(), b->getBoardY()))
		{
			b->kill(currentTime);
			killCount++;
		}
	});
	return killCount;
}

void Board::removeDeadBlocks()
{
	applyToAllBlocks([] (BlockPtr &b) {
		if(b->isDead())
		{
			b = nullptr;
		}
	});
}

void Board::simulateFalling(const unsigned int currentTime)
{
	std::uniform_int<>		block_dist(TID_BLOCK_1, TID_BLOCK_1 + NUM_BLOCK_TYPES - 1);
	int numBlocksGenerated[NUM_BLOCK_COLUMNS];
	for(int i = 0; i < NUM_BLOCK_COLUMNS; ++i)
	{
		numBlocksGenerated[i] = 0;
	}
	for(int i = 0; i < NUM_BLOCK_ROWS; ++i)
	{
		for(int j = 0; j < NUM_BLOCK_COLUMNS; ++j)
		{

			int reverseRowIndex = NUM_BLOCK_ROWS - i - 1;
			if(blocks[reverseRowIndex][j])
			{
				continue;
			}
			//search for block that can fall to this position
			bool blockFound = false;
			int testRow;
			for(testRow = reverseRowIndex - 1; testRow >= 0; --testRow)
			{
				if(!blocks[testRow][j])
				{
					continue;
				}
				if(blocks[testRow][j]->canMove())
				{
					blockFound = true;
					break;
				}
			}
			if(blockFound)
			{
				blocks[testRow][j]->fallTo(currentTime, j, reverseRowIndex);
				blocks[reverseRowIndex][j] = blocks[testRow][j];
				blocks[testRow][j] = nullptr;
			}
			else if(testRow == -1)
			{
				//generate new
				BlockPtr block(new Block(renderer));
				block->init(j, -(numBlocksGenerated[j] + 1), (TextureID)block_dist(rng));
				numBlocksGenerated[j]++;
				block->fallTo(currentTime, j, reverseRowIndex);
				blocks[reverseRowIndex][j] = block;
			}
		}
	}
}
