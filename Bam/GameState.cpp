#include "common.h"
#include "StaticWorldChunk.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "Option.h"

bool GameState::load(Loader& loader) {
	loader.retrieve(tick);
	staticWorld.load(loader);
	activityPaceHandler.load(loader);
	movementPaceHandler.load(loader);
	return false;
}

bool GameState::save(Saver& saver) {
	saver.store(tick);
	staticWorld.save(saver);
	activityPaceHandler.save(saver);
	movementPaceHandler.save(saver);
	return false;
}

void GameState::appendStaticRenderInfo(RenderInfo& renderInfo) {
	staticWorld.appendStaticRenderInfo(renderInfo);
	auto activityManager = Locator<ReferenceManager<Activity>>::get();
	for (auto& p : activityManager->data) {
		p.second.get()->appendStaticRenderInfo(*this, renderInfo.staticWorldRenderInfo);
	}
}

GameState::GameState() {
}

GameState::~GameState() {
}
