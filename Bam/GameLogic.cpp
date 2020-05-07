#include "common.h"
#include "GameLogic.h"
#include "GameState.h"

bool GameLogic::ready() {
	return glfwGetTime() > nextStep;
}

void GameLogic::runStep(GameState& gameState) {
	gameState.tick++;
	nextStep += stepTime;
	step++;

	gameState.movementPaceHandler.finish(gameState);
	gameState.movementPaceHandler.cycle();

	gameState.activityPaceHandler.finish(gameState);
	gameState.activityPaceHandler.cycle();
}
