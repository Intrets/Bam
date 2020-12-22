#include "common.h"

#include "Incinerator.h"
#include "GameState.h"
#include "WorldBlock.h"
#include "Block.h"

Incinerator::Incinerator(Handle self, glm::ivec2 pos) : SingleBlockActivity(self, pos) {
	this->baseBlock = ShapedBlock("diorite", SHAPE::TYPE::INCINERATOR, ACTIVITY::DIR::RIGHT);
}

bool Incinerator::canActivityLocal(GameState& gameState, int32_t type) {
	auto p = this->getOrigin() + ACTIVITY::GETDIRECTION(this->activityRotation);
	return gameState.staticWorld.getBlockRef(p).isNonAirBlock();
}

void Incinerator::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	pace = this->baseBlock.getBlock().getSmallRand(gameState);

	this->Activity::applyActivityLocalForced(gameState, type, pace);

	auto p = this->getOrigin() + ACTIVITY::GETDIRECTION(this->activityRotation);
	gameState.staticWorld.setBlockForce(ShapedBlock("air", "full", ACTIVITY::DIR::RIGHT), p);
}

ACTIVITY::TYPE Incinerator::getType() {
	return ACTIVITY::TYPE::INCINERATOR;
}
