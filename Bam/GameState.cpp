#include "common.h"
#include "StaticWorldChunk.h"
#include "GameState.h"
#include "RenderInfo.h"

//#include "GameObject.h"

bool GameState::load(Loader& loader) {
	loader.retrieve(tick);
	staticWorld.load(loader);
	activityPaceHandler.load(loader);
	movementPaceHandler.load(loader);
	return false;
}

bool GameState::save(Saver & saver) {
	saver.store(tick);
	staticWorld.save(saver);
	activityPaceHandler.save(saver);
	movementPaceHandler.save(saver);
	return false;
}

void GameState::appendStaticRenderInfo(RenderInfo& renderInfo) {
	staticWorld.appendStaticRenderInfo(renderInfo);
	auto activityManager = Locator<ReferenceManager<Activity>>::getService();
	for (auto& p : activityManager->data) {
		p.second.get()->appendStaticRenderInfo(*this, renderInfo.staticWorldRenderInfo);
	}
	renderInfo.debugRenderInfo.addBox(glm::vec2(1.0f, 1.0f), glm::vec2(5.0f, 5.0f));
}

GameState::GameState() {
}


GameState::~GameState() {
}
