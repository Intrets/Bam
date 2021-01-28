#include "common.h"

#include "NetworkAction.h"

#include "Saver.h"
#include "Loader.h"

#include "Client.h"
#include "Locator.h"
#include "Log.h"
#include "GameState.h"
#include "Coordinator.h"

#include <numeric>

void NetworkAction::load(Loader& loader) {
	this->loadDerived(loader);
}

void NetworkAction::save(Saver& saver) {
	saver.store(this->type);
	this->saveDerived(saver);
}

void GameLoad::loadDerived(Loader& loader) {
	int32_t size;
	loader.retrieve(size);
	assert(size > 0);

	std::vector<char> buffer(size);

	loader.getBuffer().read(buffer.data(), size);
	this->saveBuffer.write(buffer.data(), size);
}

void GameLoad::saveDerived(Saver& saver) {
	auto s = getSize(this->saveBuffer);
	assert(s > 0);
	assert(s <= std::numeric_limits<int32_t>().max());

	int32_t size = static_cast<int32_t>(s);
	saver.store(size);

	std::vector<char> buffer(size);
	this->saveBuffer.read(buffer.data(), size);
	saver.getBuffer().write(buffer.data(), size);

	this->saveBuffer.seekg(0);
}

GameLoad::GameLoad() {
	this->type = NETWORKACTION::TYPE::GAME_LOAD;
}

void GameLoad::run(GameState& gameState, COORDINATOR::Coordinator& coordinator, std::optional<Client*> maybeClient) {
	if (Loader(this->saveBuffer, gameState).loadGame()) {
		Locator<Log>::ref().putLine("Successfully loaded the world in Coordinator\n");
	}
	else {
		Locator<Log>::ref().putLine("Failed to load the world in Coordinator\n");
	}

	coordinator.reset(gameState.tick);

	if (maybeClient) {
		maybeClient.value()->uiState.clear();
		maybeClient.value()->uiState.init();
		maybeClient.value()->state.player = maybeClient.value()->state.gameState.getPlayer(0).value();
	}
}

void PlayerActions::loadDerived(Loader& loader) {
	loader.retrieve(this->uuid);
	loader.retrieve(this->tick);

	int32_t size;
	loader.retrieve(size);

	this->operations.resize(size);
	for (size_t i = 0; i < size; i++) {
		this->operations[i] = OPERATION::loadOperation(loader);
	}
}

void PlayerActions::saveDerived(Saver& saver) {
	saver.store(this->uuid);
	saver.store(this->tick);

	int32_t size = static_cast<int32_t>(this->operations.size());
	saver.store(size);

	for (auto& operation : this->operations) {
		operation->save(saver);
	}
}

PlayerActions::PlayerActions() {
	this->type = NETWORKACTION::TYPE::PLAYER_ACTIONS;
}
