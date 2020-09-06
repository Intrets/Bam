#include "common.h"
#include "SingleBlockActivity.h"

#include "StaticWorldRenderer.h"
#include "GameState.h"
#include "BlockIDTextures.h"

#include "Saver.h"
#include "Loader.h"
#include "RenderInfo.h"

#include "ActivityIgnoringGroup.h"

SingleBlockActivity::SingleBlockActivity(Handle self, GameState& gameState, glm::ivec2 pos, std::string name) : Activity(self, pos) {
	auto t = Locator<BlockIDTextures>::get();
	this->tex = t->getBlockTextureID(name);
	this->texName = name;
}

void SingleBlockActivity::rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) {
	auto d = this->getOrigin() - center;
	switch (rotation) {
		case ACTIVITY::ROT::CLOCKWISE:
			d = glm::ivec2(d.y, -d.x - 1);
			break;
		case ACTIVITY::ROT::COUNTERCLOCKWISE:
			d = glm::ivec2(-d.y - 1, d.x);
			break;
		default:
			break;
	}
	this->origin = center + d;
}

bool SingleBlockActivity::canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) {
	return !gameState.staticWorld.isOccupied(this->origin + ACTIVITY::GETDIRECTION(dir), ignore);
}

bool SingleBlockActivity::canFillTracesLocal(GameState& gameState) {
	return !gameState.staticWorld.isOccupied(this->origin);
}

void SingleBlockActivity::fillTracesLocalForced(GameState& gameState) {
	this->Activity::fillTracesLocalForced(gameState);
	gameState.staticWorld.leaveTrace(this->origin, this->selfHandle);
}

void SingleBlockActivity::removeTracesLocalForced(GameState& gameState) {
	this->Activity::removeTracesLocalForced(gameState);
	gameState.staticWorld.removeTraceForced(this->origin);
}

void SingleBlockActivity::removeMoveableTracesLocal(GameState& gameState) {
	gameState.staticWorld.removeTraceFilter(this->origin - ACTIVITY::GETDIRECTION(this->movementDirection), this->selfHandle);
}

void SingleBlockActivity::leaveMoveableTracesLocal(GameState& gameState) {
	gameState.staticWorld.leaveTrace(this->origin + ACTIVITY::GETDIRECTION(this->movementDirection), this->selfHandle);
}

void SingleBlockActivity::impl_getTreeMembers(std::vector<Activity*>& members) {
	members.push_back(this);
}

void SingleBlockActivity::impl_getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) {
	members.push_back({ depth, this });
}

void SingleBlockActivity::appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	glm::vec2 ori = this->origin;
	if (this->moving) {
		float scale = static_cast<float>(gameState.tick - this->movingTickStart) / this->movingPace;
		ori += scale * glm::vec2(ACTIVITY::GETDIRECTION(this->movementDirection));
	}
	renderInfo.selectionRenderInfo.addBox(ori, ori + glm::vec2(1), color);
}

void SingleBlockActivity::appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	glm::vec2 ori = this->origin;
	if (this->moving) {
		float scale = static_cast<float>(gameState.tick - this->movingTickStart) / this->movingPace;
		ori += scale * glm::vec2(ACTIVITY::GETDIRECTION(this->movementDirection));
	}
	staticWorldRenderInfo.addBlockWithShadow(ori, this->tex);
}

void SingleBlockActivity::save(Saver& saver) {
	this->Activity::save(saver);
	saver.storeString(this->texName);
}

bool SingleBlockActivity::load(Loader& loader) {
	this->Activity::load(loader);
	loader.retrieveString(this->texName);

	// TODO: fix up make consistent with constructor
	auto t = Locator<BlockIDTextures>::get();
	this->tex = t->getBlockTextureID(this->texName);
	return true;
}


