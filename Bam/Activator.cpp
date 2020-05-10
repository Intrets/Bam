#include "common.h"

#include "Activator.h"

#include "GameState.h"
#include <iostream>

void Activator::modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) {
}

std::stringstream& Activator::getMembers(std::stringstream& out) {
	// TODO: insert return statement here
	assert(false);
	return out;
}

Activator::Activator(Handle self, GameState& gameState, glm::ivec2 pos, std::string name, bool leaveTrace) : SingleBlockActivity(self, gameState, pos, name, leaveTrace) {
}

bool Activator::moveableIdleLocal() {
	return !moving;
}

bool Activator::activityIdleLocal() {
	return !active;
}

bool Activator::canActivityLocal(GameState& gameState, int type) {
	return true;
}

void Activator::applyActivityLocalForced(GameState& gameState, int type, int pace) {
	Activity::applyActivityLocalForced(gameState, type, pace);
	auto maybeTarget = gameState.staticWorld.getActivity(origin + glm::ivec2(1, 0));
	if (maybeTarget.has_value()) {
		maybeTarget->get()->applyActivityLocal(gameState, type, pace - 1);
	}
}

void Activator::removeActivityTracesLocal(GameState& gameState) {
	applyActivityLocal(gameState, 2, 60);
}

void Activator::leaveActivityTracesLocal(GameState& gameState) {
}

ACTIVITY::TYPE Activator::getType() {
	return ACTIVITY::TYPE::ACTIVATOR;
}

void Activator::save(Saver& saver) {
	SingleBlockActivity::save(saver);
}

bool Activator::load(Loader& loader) {
	SingleBlockActivity::load(loader);
	return true;
}
