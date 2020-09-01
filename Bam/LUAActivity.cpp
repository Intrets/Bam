#include "common.h"

#include "LuaActivity.h"
#include "GameState.h"
#include "StringHelpers.h"

void LuaActivity::start(GameState& gameState) {
	this->applyActivityLocal(gameState, 0, gameState.smallRandom.randRange(10, 20));
}

void LuaActivity::stop() {
	this->interrupt = true;
}

glm::ivec2 LuaActivity::getOrigin() {
	if (this->hasChild()) {
		return this->child.get()->getOrigin();
	}
	return glm::ivec2(0, 0);
}

LuaActivity::LuaActivity(Handle self) {
	this->selfHandle = self;
	this->inWorld = true;
}

bool LuaActivity::moveableIdleLocal() {
	return !this->moving && this->inWorld;
}

void LuaActivity::rotateForcedLocal(glm::ivec2 center, Activity::ROT rotation) {
}

bool LuaActivity::canActivityLocal(GameState& gameState, int32_t type) {
	return true;
}

void LuaActivity::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	Activity::applyActivityLocalForced(gameState, type, pace);
	this->lua.prepare(gameState);
	this->lua.run(gameState);
}

bool LuaActivity::canMoveLocal(GameState& gameState, Activity::DIR dir, ActivityIgnoringGroup& ignore) {
	return true;
}

bool LuaActivity::canFillTracesLocal(GameState& gameState) {
	return true;
}

void LuaActivity::fillTracesLocalForced(GameState& gameState) {
}

void LuaActivity::removeTracesLocalForced(GameState& gameState) {
}

void LuaActivity::removeActivityTracesLocal(GameState& gameState) {
	if (!this->interrupt) {
		this->start(gameState);
	}
	this->interrupt = false;
}

void LuaActivity::leaveActivityTracesLocal(GameState& gameState) {
	this->interrupt = false;
}

void LuaActivity::removeMoveableTracesLocal(GameState& gameState) {
}

void LuaActivity::leaveMoveableTracesLocal(GameState& gameState) {
}

Activity::TYPE LuaActivity::getType() {
	return Activity::TYPE::LUA;
}

void LuaActivity::save(Saver& saver) {
	SingleGrouper::save(saver);
}

bool LuaActivity::load(Loader& loader) {
	SingleGrouper::load(loader);
	return true;
}

void LuaActivity::appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) {
}

void LuaActivity::appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
}
