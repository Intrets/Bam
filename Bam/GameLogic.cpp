#include "common.h"
#include "GameLogic.h"
#include "GameState.h"
#include "Timer.h"
#include <iostream>

bool GameLogic::ready() {
	return glfwGetTime() > this->nextStep;
}

int GameLogic::runStep(GameState& gameState) {
	Locator<Timer>::ref().newTiming("Game Logic");
	gameState.tick++;
	this->nextStep += this->stepTime;
	this->step++;

	gameState.movementPaceHandler.finish(gameState);
	gameState.movementPaceHandler.cycle();

	gameState.activityPaceHandler.finish(gameState);
	gameState.activityPaceHandler.cycle();

	Locator<Timer>::ref().endTiming("Game Logic");
	return 0;
}
