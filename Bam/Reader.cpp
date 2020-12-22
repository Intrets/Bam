#include "common.h"

#include "Reader.h"
#include "GameState.h"
#include "WorldBlock.h"

Reader::Reader(Handle self, glm::ivec2 pos) : SingleBlockActivity(self, pos) {
	this->baseBlock = ShapedBlock("diorite", "reader_stencil", ACTIVITY::DIR::RIGHT);
}

bool Reader::canActivityLocal(GameState& gameState, int32_t type) {
	return gameState.staticWorld.getBlockRef(this->getOrigin()).isNonAirBlock();
}

void Reader::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	pace = this->baseBlock.getBlock().getSmallRand(gameState);

	this->Activity::applyActivityLocalForced(gameState, type, pace);
}

ACTIVITY::TYPE Reader::getType() {
	return ACTIVITY::TYPE::READER;
}
