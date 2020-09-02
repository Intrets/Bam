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

LuaActivity::LuaActivity(Handle self, GameState& gameState, glm::ivec2 pos, bool leaveTraces) :
	SingleBlockActivity(self, gameState, pos, "debug.dds", leaveTraces)
{
	this->inWorld = leaveTraces;
}

bool LuaActivity::moveableIdleLocal() {
	return !this->moving && this->inWorld;
}

bool LuaActivity::canActivityLocal(GameState& gameState, int32_t type) {
	return true;
}

void LuaActivity::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	Activity::applyActivityLocalForced(gameState, type, pace);
	this->lua.prepare(gameState);
	this->lua.run(gameState);
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

Activity::TYPE LuaActivity::getType() {
	return Activity::TYPE::LUA;
}

void LuaActivity::save(Saver& saver) {
	SingleBlockActivity::save(saver);
}

bool LuaActivity::load(Loader& loader) {
	SingleBlockActivity::load(loader);
	return true;
}

