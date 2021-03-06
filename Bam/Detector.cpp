#include "common.h"

#include "Detector.h"
#include "GameState.h"
#include "WorldBlock.h"

Detector::Detector(Handle self, glm::ivec2 pos) : SingleBlockActivity(self, pos) {
	this->baseBlock = ShapedBlock("diorite", "detector_stencil", ACTIVITY::DIR::RIGHT);
}

bool Detector::canActivityLocal(GameState& gameState, int32_t type) {
	auto p = this->getOrigin() + ACTIVITY::GETDIRECTION(this->activityRotation);
	return gameState.staticWorld.getBlockRef(p).isOccupied();
}

void Detector::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	pace = this->baseBlock.getBlock().getSmallRand(gameState);

	this->Activity::applyActivityLocalForced(gameState, type, pace);
}

ACTIVITY::TYPE Detector::getType() {
	return ACTIVITY::TYPE::DETECTOR;
}
