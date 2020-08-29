#include "common.h"

#include "Activator.h"

#include "GameState.h"
#include <iostream>

Activator::Activator(Handle self, GameState& gameState, glm::ivec2 pos, std::string name, bool leaveTrace) : SingleBlockActivity(self, gameState, pos, name, leaveTrace) {
}

bool Activator::moveableIdleLocal() {
	return !this->moving;
}

bool Activator::activityIdleLocal() {
	return !this->active;
}

bool Activator::canActivityLocal(GameState& gameState, int32_t type) {
	return true;
}

void Activator::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	this->Activity::applyActivityLocalForced(gameState, type, pace);
	if (auto const& maybeTarget = gameState.staticWorld.getActivity(this->origin + glm::ivec2(1, 0))) {
		maybeTarget.value().get()->applyActivityLocal(gameState, type, pace - 1);
	}
}

void Activator::removeActivityTracesLocal(GameState& gameState) {
	this->applyActivityLocal(gameState, 2, 60);
}

void Activator::leaveActivityTracesLocal(GameState& gameState) {
}

Activity::TYPE Activator::getType() {
	return Activity::TYPE::ACTIVATOR;
}

void Activator::save(Saver& saver) {
	this->SingleBlockActivity::save(saver);
}

bool Activator::load(Loader& loader) {
	this->SingleBlockActivity::load(loader);
	return true;
}
