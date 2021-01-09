#include "common.h"

#include "GameLogic.h"

#include "GameState.h"
#include "Timer.h"
#include "Locator.h"

#include <iostream>

int GameLogic::runStep(GameState& gameState) {
	Locator<Timer>::ref().newTiming("Game Logic");
	gameState.tick++;

	gameState.movementPaceHandler.finish(gameState);
	gameState.movementPaceHandler.cycle();

	gameState.activityPaceHandler.finish(gameState);
	gameState.activityPaceHandler.cycle();

	Locator<Timer>::ref().endTiming("Game Logic");
	return 0;
}
