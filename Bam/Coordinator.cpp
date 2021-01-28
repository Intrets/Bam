#include "common.h"

#include "Coordinator.h"

#include "NetworkLayer.h"
#include "Saver.h"
#include "Loader.h"
#include "NetworkAction.h"

#include <sstream>

void COORDINATOR::Coordinator::pushTick(int32_t tick, PlayerActions&& playerActions) {
	PlayerActions actions;
	actions = std::move(playerActions);

	if (this->gameStateUuid != actions.uuid) {
		return;
	}

	if (this->tickBuffer.find(this->currentTick) == this->tickBuffer.end()) {
		this->tickBuffer[this->currentTick] = std::move(actions);
		this->tickBuffer[this->currentTick].uuid = this->gameStateUuid;
	}

	auto& operations = this->tickBuffer[this->currentTick].operations;

	operations.insert(
		operations.begin(),
		std::make_move_iterator(actions.operations.begin()),
		std::make_move_iterator(actions.operations.end())
	);
}

void COORDINATOR::Coordinator::pushMessage(NETWORK::Message&& message) {
	std::lock_guard<std::mutex> lock(this->mutex);

	Loader loader(message.buffer);
	NETWORKACTION::TYPE type;

	loader.retrieve(type);

	switch (type) {
		case NETWORKACTION::TYPE::PLAYER_ACTIONS:
			{
				PlayerActions playerActions;
				playerActions.load(loader);

				this->pushTick(this->currentTick, std::move(playerActions));
				break;
			}
		case NETWORKACTION::TYPE::GAME_LOAD:
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
