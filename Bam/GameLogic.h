#pragma once

class GameState;

class GameLogic
{
private:
	double nextStep = 0.0;
	double stepTime = 1 / 60.0;

	int32_t step = 0;

public:
	bool ready();

	void runStep(GameState& gameState);
};

