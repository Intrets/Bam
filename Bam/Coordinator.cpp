#include "common.h"

#include "Coordinator.h"

#include "NetworkLayer.h"
#include "Saver.h"
#include "Loader.h"
#include "MetaOperation.h"

#include <sstream>

std::ostream& operator<<(std::ostream& out, COORDINATOR::MESSAGE::TYPE const& type) {
	out << static_cast<int32_t>(type);
	return out;
}

std::istream& operator>>(std::istream& in, COORDINATOR::MESSAGE::TYPE& type) {
	in.read(reinterpret_cast<char*>(&type), sizeof(int32_t));
	return in;
}

void COORDINATOR::Coordinator::pushTick(int32_t tick, PlayerActions&& playerActions) {
	PlayerActions actions = std::move(playerActions);

	std::stringstream buffer;

	buffer << COORDINATOR::MESSAGE::TYPE::PLAYER_ACTIONS;

	Saver saver(buffer);
	actions.save(saver);

	auto& op = this->tickBuffer[this->currentTick].operations;

	for (auto& operation : actions.operations) {
		op.push_back(std::move(operation));
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
				}

				PlayerActions playerActions;
				playerActions.load(loader);

				//if (playerActions.uuid != this->tickBuffer[this->currentTick])

				//this->tickBuffer[this->currentTick].append(loader);
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
