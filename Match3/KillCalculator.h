#ifndef _KILL_CALCULATOR_H_
#define _KILL_CALCULATOR_H_

class KillCalculator
{
	int						blockTypes[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS];
	bool					blockKills[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS];

	void initBlockTypes(const Board &board);
	int getBlockType(int x, int y) const;
	bool verticalKillSearch(int x, int y) const;
	bool horizontalKillSearch(int x, int y) const;
public:
	KillCalculator(const Board &board);

	void swapTypes(int srcX, int srcY, int dstX, int dstY);
	void calculateKills();
	bool hasKills() const;
	bool hasKillAt(int x, int y) const;
};

#endif
