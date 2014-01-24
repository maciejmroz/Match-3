#include "GameImpl.h"

KillCalculator::KillCalculator(const Board &board)
{
	initBlockTypes(board);
}

void KillCalculator::initBlockTypes(const Board &board)
{
	board.mapBlocks<int>(blockTypes, [](BlockPtr b) -> int {
		if(!b)
		{
			return -1;
		}
		return b->getType();
	});
}

int KillCalculator::getBlockType(int x, int y) const
{
	if(y < 0 || y >= NUM_BLOCK_ROWS ||
		x < 0 || x >= NUM_BLOCK_COLUMNS)
	{
		return -1;
	}
	return blockTypes[y][x];
}

void KillCalculator::swapTypes(int srcX, int srcY, int dstX, int dstY)
{
	std::swap(blockTypes[dstY][dstX], blockTypes[srcY][srcX]);
}

bool KillCalculator::verticalKillSearch(int x, int y) const
{
	for(int baseY = 0; baseY <= 2; ++baseY)
	{
		if((getBlockType(x, y + baseY - 2) == getBlockType(x, y + baseY - 1)) &&
			(getBlockType(x, y + baseY - 1) == getBlockType(x, y + baseY)))
		{
			return true;
		}
	}
	return false;
}

bool KillCalculator::horizontalKillSearch(int x, int y) const
{
	for(int baseX = 0; baseX <= 2; ++baseX)
	{
		if((getBlockType(x + baseX - 2, y) == getBlockType(x + baseX - 1, y)) &&
			(getBlockType(x + baseX - 1, y) == getBlockType(x + baseX, y)))
		{
			return true;
		}
	}
	return false;
}

void KillCalculator::calculateKills()
{
	for(int i = 0; i < NUM_BLOCK_ROWS; ++i)
	{
		for(int j = 0; j < NUM_BLOCK_COLUMNS; ++j)
		{
			blockKills[i][j] = false;
			if(getBlockType(j, i) == -1)
			{
				continue;
			}
			if(horizontalKillSearch(j, i) || verticalKillSearch(j, i))
			{
				blockKills[i][j] = true;
			}
		}
	}
}

bool KillCalculator::hasKills() const
{
	for(int i = 0; i < NUM_BLOCK_ROWS; ++i)
	{
		for(int j = 0; j < NUM_BLOCK_COLUMNS; ++j)
		{
			if(blockKills[i][j])
			{
				return true;
			}
		}
	}
	return false;
}

bool KillCalculator::hasKillAt(int x, int y) const
{
	return blockKills[y][x];
}
