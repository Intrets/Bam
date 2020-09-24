#include "common.h"

#include "Reader.h"
#include "GameState.h"
#include "WorldBlock.h"

Reader::Reader(Handle self, glm::ivec2 pos) : SingleBlockActivity(self, pos, "reader.dds") {
}

bool Reader::canActivityLocal(GameState& gameState, int32_t type) {
	return gameState.staticWorld.getBlockRef(this->getOrigin())->isNonAirBlock();
}

void Reader::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	pace = this->material.getSmallRand(gameState);

	this->Activity::applyActivityLocalForced(gameState, type, pace);
}

ACTIVITY::TYPE Reader::getType() {
	return ACTIVITY::TYPE::READER;
}
