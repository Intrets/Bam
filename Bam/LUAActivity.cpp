#include "common.h"

#include "LUAActivity.h"

LUAActivity::LUAActivity(Handle self) {
	this->selfHandle = self;
}

void LUAActivity::rotateForcedLocal(glm::ivec2 center, Activity::ROT rotation) {
}

bool LUAActivity::canActivityLocal(GameState& gameState, int32_t type) {
	return false;
}

void LUAActivity::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
}

bool LUAActivity::canMoveLocal(GameState& gameState, Activity::DIR dir, ActivityIgnoringGroup& ignore) {
	return true;
}

bool LUAActivity::canFillTracesLocal(GameState& gameState) {
	return true;
}

void LUAActivity::fillTracesLocalForced(GameState& gameState) {
}

void LUAActivity::removeTracesLocalForced(GameState& gameState) {
}

void LUAActivity::removeActivityTracesLocal(GameState& gameState) {
}

void LUAActivity::leaveActivityTracesLocal(GameState& gameState) {
}

void LUAActivity::removeMoveableTracesLocal(GameState& gameState) {
}

void LUAActivity::leaveMoveableTracesLocal(GameState& gameState) {
}

Activity::TYPE LUAActivity::getType() {
	return Activity::TYPE::LUA;
}

void LUAActivity::save(Saver& saver) {
	SingleGrouper::save(saver);
}

bool LUAActivity::load(Loader& loader) {
	SingleGrouper::load(loader);
	return true;
}

void LUAActivity::appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) {
}

void LUAActivity::appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
}
