#include "common.h"

#include "StaticWorldChunk.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "Option.h"
#include "Saver.h"
#include "Loader.h"
#include "Timer.h"
#include "BlockIDTextures.h"

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
	Locator<Timer>::ref().newTiming("Prepare Static");
	this->staticWorld.appendStaticRenderInfo(renderInfo);
	Locator<Timer>::ref().endTiming("Prepare Static");

	// TODO: culling, seperate from static world
	Locator<Timer>::ref().newTiming("Prep Activities");
	auto activityManager = Locator<ReferenceManager<Activity>>::get();
	int32_t highlightBlockID = Locator<BlockIDTextures>::ref().getBlockTextureID("highlight.dds");
	for (auto& p : activityManager->data) {
		p.second.get()->appendStaticRenderInfo(*this, renderInfo.staticWorldRenderInfo);
		if (!p.second.get()->idleLocal()) {
			renderInfo.staticWorldRenderInfo.addBlockWithoutShadow(p.second.get()->getMovingOrigin(*this), highlightBlockID);
		}
	}
	Locator<Timer>::ref().endTiming("Prep Activities");
}

GameState::GameState() {
}

GameState::~GameState() {
}
