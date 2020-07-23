#include "common.h"
#include "GameLogic.h"
#include "GameState.h"
#include <iostream>

bool GameLogic::ready() {
	return glfwGetTime() > this->nextStep;
}

int GameLogic::runStep(GameState& gameState) {
	gameState.tick++;
	this->nextStep += this->stepTime;
	this->step++;

	gameState.movementPaceHandler.finish(gameState);
	gameState.movementPaceHandler.cycle();

	gameState.activityPaceHandler.finish(gameState);
	gameState.activityPaceHandler.cycle();

	return 0;
}
