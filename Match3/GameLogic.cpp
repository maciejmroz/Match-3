#include "GameImpl.h"

bool Game::impl::tryGameStart(unsigned int currentTime)
{
	if(!firstGame && (currentTime - gameStopTime < POST_GAME_TIME * 1000))
	{
		return false;
	}
	if(!firstGame)
	{
		board->generate();
	}
	gameStarted = true;
	gameStartTime = currentTime;
	timeLeftSeconds = TIME_LIMIT;
	score = 0;
	firstGame = false;
	return true;
}

void Game::impl::simulate(unsigned int currentTime)
{
	if(firstGame && !gameStarted)
	{
		return;
	}

	int killCount = board->simulateKills(currentTime);
	if(killCount)
	{
		score += 2 + (killCount - 1) * (killCount - 2) / 2;
	}

	board->removeDeadBlocks();

	board->simulateFalling(currentTime);

	if(gameStarted)
	{
		if(currentTime - gameStartTime > TIME_LIMIT * 1000)
		{
			gameStopTime = currentTime;
			gameStarted = false;
			board->mouseDownBlock = nullptr;
			timeLeftSeconds = 0;
		}
		else
		{
			timeLeftSeconds = TIME_LIMIT - (currentTime - gameStartTime) / 1000; 
		}
	}
}
