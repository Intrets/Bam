#include "common.h"

#include "StaticWorldChunk.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "Option.h"
#include "Saver.h"
#include "Loader.h"

bool GameState::load(Loader& loader) {
	loader.retrieve(this->tick);
	this->staticWorld.load(loader);
	this->activityPaceHandler.load(loader);
	this->movementPaceHandler.load(loader);
	return false;
}

bool GameState::save(Saver& saver) {
	saver.store(this->tick);
	this->staticWorld.save(saver);
	this->activityPaceHandler.save(saver);
	this->movementPaceHandler.save(saver);
	return false;
}

void GameState::appendStaticRenderInfo(RenderInfo& renderInfo) {
	this->staticWorld.appendStaticRenderInfo(renderInfo);
	// TODO: culling, seperate from static world
	auto activityManager = Locator<ReferenceManager<Activity>>::get();
	for (auto& p : activityManager->data) {
		p.second.get()->appendStaticRenderInfo(*this, renderInfo.staticWorldRenderInfo);
	}
}

GameState::GameState() {
}

GameState::~GameState() {
}
