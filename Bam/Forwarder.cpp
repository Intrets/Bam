#include "common.h"

#include "Forwarder.h"
#include "GameState.h"
#include "WorldBlock.h"
#include "LuaActivity.h"

bool Forwarder::receiveMessage(GameState& gameState, LUASTORE::Args& args_) {
	if (!this->activityIdleLocal()){
		this->applyActivityLocalForced(gameState, 0, 10);
		this->args = std::move(args_);
		return true;
	}
	else {
		return false;
	}
}

bool Forwarder::canReceiveMessage(GameState& gameState) {
	return this->activityIdleLocal();
}

Forwarder::Forwarder(Handle self, glm::ivec2 pos) : SingleBlockActivity(self, pos, "forwarder.dds") {
}

bool Forwarder::canActivityLocal(GameState& gameState, int32_t type) {
	return false;
}

void Forwarder::postActivityStopLocal(GameState& gameState) {
	auto p = this->getOrigin() + ACTIVITY::GETDIRECTION(this->activityRotation);
	auto block = gameState.staticWorld.getBlockRef(p);
	if (!block->isActivity()) {
		this->applyActivityLocalForced(gameState, 0, 10);
		return;
	}

	auto activity = block->getActivity().get();
	if (activity == this) {
		return;
	}

	switch (activity->getType()) {
		case ACTIVITY::TYPE::LUA:
			if (!static_cast<LuaActivity*>(activity)->receiveMessage(this->args)) {
				this->applyActivityLocalForced(gameState, 0, 10);
			}
			break;
		case ACTIVITY::TYPE::FORWARDER:
			if (!static_cast<Forwarder*>(activity)->receiveMessage(gameState, this->args)) {
				this->applyActivityLocalForced(gameState, 0, 10);
			}
			break;
		default:
			this->applyActivityLocalForced(gameState, 0, 10);
			break;
	}
}

ACTIVITY::TYPE Forwarder::getType() {
	return ACTIVITY::TYPE::FORWARDER;
}

void Forwarder::save(Saver& saver) {
	this->SingleBlockActivity::save(saver);
	this->args.save(saver);
}

bool Forwarder::load(Loader& loader) {
	this->SingleBlockActivity::load(loader);
	this->args.load(loader);
	return true;
}

void Forwarder::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	pace = this->material.getSmallRand(gameState);

	this->Activity::applyActivityLocalForced(gameState, type, pace);
}
