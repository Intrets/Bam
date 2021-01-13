#include "common.h"

#include "MetaOperation.h"

#include "Saver.h"
#include "Loader.h"
#include "Locator.h"
#include "Log.h"
#include "GameState.h"
#include "Coordinator.h"
#include "Client.h"

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

void GameLoad::loadDerived(Loader& loader) {
	this->saveBuffer << loader.getBuffer().rdbuf();
}

void GameLoad::saveDerived(Saver& saver) {
	saver.getBuffer() << this->saveBuffer.rdbuf();
}

GameLoad::GameLoad() {
	this->type = COORDINATOR::MESSAGE::TYPE::GAME_LOAD;
}

#define LOAD(TYPE) res = std::make_unique<TYPE>();\
				res->type = type;\
				res->load(loader);

std::unique_ptr<MetaOperation> METAOPERATION::loadOperation(Loader& loader) {
	int32_t t;
	loader.retrieve(t);
	auto type = static_cast<COORDINATOR::MESSAGE::TYPE>(t);

	std::unique_ptr<MetaOperation> res;

	switch (type) {
		case COORDINATOR::MESSAGE::TYPE::GAME_LOAD:
			LOAD(GameLoad)
				break;
		default:
			assert(0);
			break;
	}

	return res;
}

void MetaOperation::load(Loader& loader) {
	this->loadDerived(loader);
}

void MetaOperation::save(Saver& saver) {
	saver.store(static_cast<int32_t>(this->type));
	this->saveDerived(saver);
}
