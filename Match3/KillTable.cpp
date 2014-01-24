#include "GameImpl.h"

KillTable::KillTable(BlockPtr blocks[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS])
{
	ITERATE_ALL_BEGIN
		blockKills[i][j] = false;
	ITERATE_ALL_END
	initBlockTypes(blocks);
}

void KillTable::initBlockTypes(BlockPtr blocks[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS])
{
	ITERATE_ALL_BEGIN
		if(!blocks[i][j])
		{
			blockTypes[i][j] = -1;
		}
		else
		{
			blockTypes[i][j] = blocks[i][j]->getType();
		}
	ITERATE_ALL_END
}

int KillTable::getBlockType(int x, int y) const
{
	if(y < 0 || y >= NUM_BLOCK_ROWS ||
		x < 0 || x >= NUM_BLOCK_COLUMNS)
	{
		return -1;
	}
	return blockTypes[y][x];
}

void KillTable::swapTypes(int srcX, int srcY, int dstX, int dstY)
{
	std::swap(blockTypes[dstY][dstX], blockTypes[srcY][srcX]);
}

void KillTable::calculateKills()
{
	ITERATE_ALL_BEGIN
		if(getBlockType(j, i) == -1)
		{
			continue;
		}
		//horizontal search
		for(int baseX = 0; baseX <= 2; ++baseX)
		{
			if((getBlockType(j + baseX - 2, i) == getBlockType(j + baseX - 1, i)) &&
				(getBlockType(j + baseX - 1, i) == getBlockType(j + baseX, i)))
			{
				blockKills[i][j] = true;
			}
		}
		//vertical search
		for(int baseY = 0; baseY <= 2; ++baseY)
		{
			if((getBlockType(j, i + baseY - 2) == getBlockType(j, i + baseY - 1)) &&
				(getBlockType(j, i + baseY - 1) == getBlockType(j, i + baseY)))
			{
				blockKills[i][j] = true;
			}
		}
	ITERATE_ALL_END
}

bool KillTable::hasKills() const
{
	ITERATE_ALL_BEGIN
		if(blockKills[i][j])
		{
			return true;
		}
	ITERATE_ALL_END
	return false;
}

bool KillTable::hasKillAt(int x, int y) const
{
	return blockKills[y][x];
}
