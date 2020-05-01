#include "common.h"
#include "GameLogic.h"

bool GameLogic::ready() {
	return glfwGetTime() > nextStep;
}

void GameLogic::runStep(GameState& gameState) {
	nextStep += stepTime;
	step++;
}
