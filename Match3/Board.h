#ifndef _BOARD_H_
#define _BOARD_H_

template<typename SrcT, typename DstT>
void mapTable(SrcT mapFrom[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS],
			  DstT mapTo[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS],
			  const std::function<DstT (const SrcT&)> &f)
{
	for(int i = 0; i < NUM_BLOCK_ROWS; ++i)
	{
		for(int j = 0; j < NUM_BLOCK_COLUMNS; ++j)
		{
			mapTo[i][j] = f(mapFrom[i][j]);
		}
	}
}

struct Board
{
	Renderer				&renderer;
	std::mt19937			rng;

	BlockPtr				blocks[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS];
	BlockPtr				mouseDownBlock;

	Board(Renderer &renderer);

	void applyToAllBlocks(const std::function<void (BlockPtr&)> &f);
	BlockPtr findBlock(const std::function<bool (BlockPtr)> &predicate);

	template<typename T>
	void mapBlocks(T mapTo[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS],
		const std::function<T (const BlockPtr&)> &f) const
	{
		mapTable<const BlockPtr, T>(blocks, mapTo, f);
	}

//board logic
	void generate();
	//return number of blocks killed
	int simulateKills(unsigned int currentTime);
	void removeDeadBlocks();
	//check for falling blocks/new blocks
	void simulateFalling(unsigned int currentTime);
};

typedef std::shared_ptr<Board> BoardPtr;

#endif