#include "common.h"

#include "StaticWorldChunk.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "Option.h"
#include "Saver.h"
#include "Loader.h"
#include "Timer.h"
#include "BlockIDTextures.h"
#include "Uuid.h"

ReferenceManager<Activity>& GameState::getActivityManager() {
	return this->activityManager;
}

ReferenceManager<InventoryItem>& GameState::getInventoryItemManager() {
	return this->inventoryItemManager;
}

std::optional<Player*> GameState::getPlayer(int32_t index) {
	if (indexInVector(index, this->players)) {
		return this->players[index].get();
	}
	return std::nullopt;
}

void GameState::makePlayer() {
	this->players.push_back(std::make_unique<Player>());
}

bool GameState::load(Loader& loader) {
	loader.retrieve(this->tick);
	this->staticWorld.load(loader);
	this->activityPaceHandler.load(loader);
	this->movementPaceHandler.load(loader);

	loader.retrieve(this->uuid);

	size_t size;
	loader.retrieve(size);
	this->players.clear();
	for (size_t i = 0; i < size; i++) {
		this->players.push_back(std::make_unique<Player>());
		this->players.back().get()->load(loader);
	}

	return true;
}

bool GameState::save(Saver& saver) {
	saver.store(this->tick);
	this->staticWorld.save(saver);
	this->activityPaceHandler.save(saver);
	this->movementPaceHandler.save(saver);

	saver.store(this->uuid);

	saver.store(this->players.size());
	for (auto& player : this->players) {
		player.get()->save(saver);
	}

	return true;
}

void GameState::clear() {
	this->getInventoryItemManager().clear();

	while (!this->getActivityManager().data.empty()) {
		auto& [handle, begin] = *this->getActivityManager().data.begin();

		begin.get()->getRootRef().deleteObject();
	}

	this->getActivityManager().clear();
	this->uuid = UUID::getUUID();
}

void GameState::appendStaticRenderInfo(RenderInfo& renderInfo) {
	Locator<Timer>::ref().newTiming("Prepare Static");
	this->staticWorld.appendStaticRenderInfo(renderInfo);
	Locator<Timer>::ref().endTiming("Prepare Static");

	// TODO: culling, seperate from static world
	Locator<Timer>::ref().newTiming("Prep Activities");
	//int32_t highlightBlockID = Locator<BlockIDTextures>::ref().getBlockTextureID("highlight.dds");
	for (auto& p : this->activityManager.data) {
		auto activity = p.second.get();
		if (activity->isInWorld()) {
			activity->appendStaticRenderInfo(*this, renderInfo.staticWorldRenderInfo);
			if (!activity->idleLocal()) {
				// TODO: re-add rendering of a block to show activity is active
				//renderInfo.staticWorldRenderInfo.addBlockWithoutShadow(p.second.get()->getMovingOrigin(*this), highlightBlockID);
			}

		}
	}
	Locator<Timer>::ref().endTiming("Prep Activities");
}

GameState::GameState() {
	this->uuid = UUID::getUUID();
}

GameState::~GameState() {
	this->clear();
}
