#include "common.h"

#include "Coordinator.h"

#include "NetworkLayer.h"
#include "Saver.h"
#include "Loader.h"
#include "MetaOperation.h"

#include <sstream>

void COORDINATOR::Coordinator::pushTick(int32_t tick, PlayerActions&& playerActions) {
	PlayerActions actions = std::move(playerActions);

	auto& op = this->tickBuffer[this->currentTick].operations;

	if (this->tickBuffer.find(tick) == this->tickBuffer.end()) {
		this->tickBuffer[tick] = std::move(actions);
	}
	else {
		if (this->gameStateUuid == actions.uuid) {
			this->tickBuffer[tick].append(actions);
		}
	}
}

void COORDINATOR::Coordinator::pushMessage(NETWORK::Message&& message) {
	std::lock_guard<std::mutex> lock(this->mutex);

	COORDINATOR::MESSAGE::TYPE type;

	message.buffer >> type;

	Loader loader(message.buffer);

	switch (type) {
		case COORDINATOR::MESSAGE::TYPE::PLAYER_ACTIONS:
			{
				if (this->tickBuffer.find(this->currentTick) == this->tickBuffer.end()) {
					this->tickBuffer[this->currentTick] = PlayerActions();
					this->tickBuffer[this->currentTick].uuid = this->gameStateUuid;
				}

				PlayerActions playerActions;
				playerActions.load(loader);

				if (this->gameStateUuid != playerActions.uuid) {
					return;
				}

				this->tickBuffer[this->currentTick].append(playerActions);
				break;
			}
		case COORDINATOR::MESSAGE::TYPE::GAME_LOAD:
			{
				std::unique_ptr<GameLoad> op = std::make_unique<GameLoad>();
				op->load(loader);

				this->maybeLoadGame = std::move(op);
				break;
			}
		default:
			assert(0);
			break;
	}

}

bool COORDINATOR::Coordinator::ready(int32_t tick) {
	std::lock_guard<std::mutex> lock(this->mutex);
	return this->currentTick == tick;
}

PlayerActions COORDINATOR::Coordinator::pullTick() {
	std::lock_guard<std::mutex> lock(this->mutex);

	PlayerActions res;
	auto resIt = this->tickBuffer.find(this->currentTick);
	if (resIt != this->tickBuffer.end()) {
		res = std::move(resIt->second);
	}
	else {
		res.tick = this->currentTick;
	}

	this->currentTick++;
	auto it = this->tickBuffer.find(this->currentTick - this->tickBufferSize);
	if (it != this->tickBuffer.end()) {
		this->tickBuffer.erase(it);
	}

	return std::move(res);
}

void COORDINATOR::Coordinator::reset(int32_t tick) {
	std::lock_guard<std::mutex> lock(this->mutex);
	this->currentTick = tick;
	this->tickBuffer.clear();
}
