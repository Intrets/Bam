#include "common.h"
#include "SingleBlockActivity.h"

#include "StaticWorldRenderer.h"
#include "GameState.h"
#include "BlockIDTextures.h"

#include "Saver.h"
#include "Loader.h"
#include "RenderInfo.h"

#include "ActivityIgnoringGroup.h"

SingleBlockActivity::SingleBlockActivity(Handle self, GameState& gameState, glm::ivec2 pos, std::string name, bool leaveTrace) : Activity(self, pos) {
	auto t = Locator<BlockIDTextures>::getService();
	tex = t->getBlockTextureID(name);
	texName = name;
	if (leaveTrace) {
		fillTracesLocalForced(gameState);
	}
}

bool SingleBlockActivity::canMoveLocal(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) {
	return !gameState.staticWorld.isOccupied(origin + MOVEABLE::DIRECTION[dir], ignore);
}

bool SingleBlockActivity::canFillTracesLocal(GameState& gameState) {
	return !gameState.staticWorld.isOccupied(origin);
}

void SingleBlockActivity::fillTracesLocalForced(GameState& gameState) {
	gameState.staticWorld.leaveTrace(origin, selfHandle);
}

void SingleBlockActivity::removeTracesLocalForced(GameState& gameState) {
	gameState.staticWorld.removeTraceForced(origin);
}

void SingleBlockActivity::removeMoveableTracesLocal(GameState& gameState) {
	gameState.staticWorld.removeTraceForced(origin - getDirection(movementDirection));
}

void SingleBlockActivity::leaveMoveableTracesLocal(GameState& gameState) {
	gameState.staticWorld.leaveTrace(origin + getDirection(movementDirection), selfHandle);
}

void SingleBlockActivity::getTreeMembers(std::vector<Activity*>& members) {
	members.push_back(this);
}

void SingleBlockActivity::appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo) {
	glm::vec2 ori = origin;
	if (moving) {
		float scale = static_cast<float>(gameState.tick - movingTickStart) / movingPace;
		ori += scale * glm::vec2(getDirection(movementDirection));
	}
	renderInfo.selectionRenderInfo.addBox(ori, ori + glm::vec2(1));
}

void SingleBlockActivity::appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	glm::vec2 ori = origin;
	if (moving) {
		float scale = static_cast<float>(gameState.tick - movingTickStart) / movingPace;
		ori += scale * glm::vec2(getDirection(movementDirection));
	}
	staticWorldRenderInfo.addBlockWithShadow(ori, tex);
}

void SingleBlockActivity::fillModifyingMap(ModifyerBase& modifyer) {
	Activity::fillModifyingMap(modifyer);
}

void SingleBlockActivity::save(Saver& saver) {
	Activity::save(saver);
	saver.storeString(texName);
}

bool SingleBlockActivity::load(Loader& loader) {
	Activity::load(loader);
	loader.retrieveString(texName);

	// TODO: fix up make consistent with constructor
	auto t = Locator<BlockIDTextures>::getService();
	tex = t->getBlockTextureID(texName);
	return true;
}


