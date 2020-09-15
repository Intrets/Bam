#include "common.h"

#include "Reader.h"
#include "GameState.h"
#include "Block.h"

Reader::Reader(Handle self, glm::ivec2 pos) : SingleBlockActivity(self, pos, "reader.dds") {
}

bool Reader::canActivityLocal(GameState& gameState, int32_t type) {
	return gameState.staticWorld.getBlockRef(this->getOrigin())->isNonAirBlock();
}

void Reader::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
}

void Reader::removeActivityTracesLocal(GameState& gameState) {
}

void Reader::leaveActivityTracesLocal(GameState& gameState) {
}

ACTIVITY::TYPE Reader::getType() {
	return ACTIVITY::TYPE::READER;
}
