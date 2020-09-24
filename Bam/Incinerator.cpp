#include "common.h"

#include "Incinerator.h"
#include "GameState.h"
#include "WorldBlock.h"
#include "Block.h"

Incinerator::Incinerator(Handle self, glm::ivec2 pos) : SingleBlockActivity(self, pos, "incinerator.dds") {
}

bool Incinerator::canActivityLocal(GameState& gameState, int32_t type) {
	auto p = this->getOrigin() + ACTIVITY::GETDIRECTION(this->activityRotation);
	return gameState.staticWorld.getBlockRef(p)->isNonAirBlock();
}

void Incinerator::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	pace = this->material.getSmallRand(gameState);

	this->Activity::applyActivityLocalForced(gameState, type, pace);

	auto p = this->getOrigin() + ACTIVITY::GETDIRECTION(this->activityRotation);
	gameState.staticWorld.setBlock(ShapedBlock("air", "full", ACTIVITY::DIR::RIGHT), p);
}

ACTIVITY::TYPE Incinerator::getType() {
	return ACTIVITY::TYPE::INCINERATOR;
}
