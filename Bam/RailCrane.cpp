#include "common.h"

#include "RailCrane.h"
#include "BlockIDTextures.h"
#include "GameState.h"
#include "StaticWorldRenderInfo.h"
#include "RenderInfo.h"
#include "ActivityIgnoringGroup.h"
#include "Anchor.h"
#include "Saver.h"
#include "Loader.h"

RailCrane::RailCrane(Handle self, GameState& gameState, glm::ivec2 pos) :
	SingleGrouper(self, pos) {
	this->length = 6;
	this->orientation = ACTIVITY::DIR::RIGHT;
	this->anchorDirection = RAILCRANE::DIR::STATIONARY;
	this->anchorIndexPos = 0;

	auto t = Locator<BlockIDTextures>::get();
	this->anchorTex = t->getBlockTextureID("crane_anchor.dds");
	this->shaftTex = t->getBlockTextureID("crane_shaft.dds");
	this->supportTex = t->getBlockTextureID("crane_support.dds");
}

void RailCrane::rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) {
	auto d = this->origin - center;
	switch (rotation) {
		case ACTIVITY::ROT::CLOCKWISE:
			d = glm::ivec2(d.y, -d.x - 1);
			this->orientation = ACTIVITY::CLOCKWISEROT(this->orientation);
			break;
		case ACTIVITY::ROT::COUNTERCLOCKWISE:
			this->orientation = ACTIVITY::COUNTERWISEROT(this->orientation);
			d = glm::ivec2(-d.y - 1, d.x);
			break;
		default:
			break;
	}
	this->origin = center + d;
}

bool RailCrane::canActivityLocal(GameState& gameState, int32_t type) {
	switch (static_cast<RAILCRANE::DIR>(type)) {
		case RAILCRANE::DIR::LEFT:
			{
				if (this->anchorIndexPos == 0) {
					return false;
				}
				if (this->child.isNotNull()) {
					return this->child.get()->canMoveUp(gameState, ACTIVITY::FLIP(this->orientation));
				}
				return true;
				break;
			}
		case RAILCRANE::DIR::RIGHT:
			{
				if (this->anchorIndexPos == this->length - 1) {
					return false;
				}
				if (this->child.isNotNull()) {
					return this->child.get()->canMoveUp(gameState, this->orientation);
				}
				return true;
				break;
			}
		default:
			break;
	}
	return false;
}

void RailCrane::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	this->Activity::applyActivityLocalForced(gameState, type, pace);
	switch (static_cast<RAILCRANE::DIR>(type)) {
		case RAILCRANE::DIR::LEFT:
			if (this->child.isNotNull()) {
				this->child.get()->applyMoveUpForced(gameState, ACTIVITY::FLIP(this->orientation), pace);
			}
			this->anchorDirection = RAILCRANE::DIR::LEFT;
			this->anchorIndexPos--;
			break;
		case RAILCRANE::DIR::RIGHT:
			if (this->child.isNotNull()) {
				this->child.get()->applyMoveUpForced(gameState, this->orientation, pace);
			}
			this->anchorDirection = RAILCRANE::DIR::RIGHT;
			this->anchorIndexPos++;
			break;
		default:
			break;
	}
}

void RailCrane::forceChangeActivityState(int32_t s) {
	switch (static_cast<RAILCRANE::DIR>(s)) {
		case RAILCRANE::DIR::LEFT:
			this->anchorIndexPos = glm::max(0, this->anchorIndexPos - 1);
			break;
		case RAILCRANE::DIR::RIGHT:
			this->anchorIndexPos = glm::min(this->anchorIndexPos + 1, this->length - 1);
			break;
		case RAILCRANE::DIR::STATIONARY:
			break;
		default:
			break;
	}
}

bool RailCrane::canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) {
	auto d = ACTIVITY::GETDIRECTION(this->orientation);
	auto pos = this->origin + ACTIVITY::GETDIRECTION(dir);
	return !gameState.staticWorld.isOccupied(pos, ignore)
		&& !gameState.staticWorld.isOccupied(pos + d * (1 + this->length), ignore);
}

bool RailCrane::canFillTracesLocal(GameState& gameState) {
	auto dir = ACTIVITY::GETDIRECTION(this->orientation);
	return !gameState.staticWorld.isOccupied(this->origin)
		&& !gameState.staticWorld.isOccupied(this->origin + dir * (1 + this->length));
}

void RailCrane::fillTracesLocalForced(GameState& gameState) {
	this->Activity::fillTracesLocalForced(gameState);
	auto dir = ACTIVITY::GETDIRECTION(this->orientation);
	gameState.staticWorld.leaveTrace(this->origin, this->selfHandle);
	gameState.staticWorld.leaveTrace(this->origin + dir * (1 + this->length), this->selfHandle);
}

void RailCrane::removeTracesLocalForced(GameState& gameState) {
	this->Activity::removeTracesLocalForced(gameState);
	auto dir = ACTIVITY::GETDIRECTION(this->orientation);
	gameState.staticWorld.removeTraceForced(this->origin);
	gameState.staticWorld.removeTraceForced(this->origin + dir * (1 + this->length));
}

void RailCrane::removeActivityTracesLocal(GameState& gameState) {
}

void RailCrane::leaveActivityTracesLocal(GameState& gameState) {
}

void RailCrane::removeMoveableTracesLocal(GameState& gameState) {
	auto d = ACTIVITY::GETDIRECTION(this->orientation);
	auto pos = this->origin + ACTIVITY::GETDIRECTION(ACTIVITY::FLIP(this->movementDirection));
	gameState.staticWorld.removeTraceFilter(pos, this->selfHandle);
	gameState.staticWorld.removeTraceFilter(pos + d * (1 + this->length), this->selfHandle);
}

void RailCrane::leaveMoveableTracesLocal(GameState& gameState) {
	auto d = ACTIVITY::GETDIRECTION(this->orientation);
	auto pos = this->origin + ACTIVITY::GETDIRECTION(this->movementDirection);
	gameState.staticWorld.leaveTrace(pos, this->selfHandle);
	gameState.staticWorld.leaveTrace(pos + d * (1 + this->length), this->selfHandle);
}

ACTIVITY::TYPE RailCrane::getType() {
	return ACTIVITY::TYPE::RAILCRANE;
}

void RailCrane::save(Saver& saver) {
	this->SingleGrouper::save(saver);
	saver.store(this->orientation);
	saver.store(this->anchorDirection);
	saver.store(this->length);
	saver.store(this->anchorIndexPos);
}

bool RailCrane::load(Loader& loader) {
	this->SingleGrouper::load(loader);
	loader.retrieve(this->orientation);
	loader.retrieve(this->anchorDirection);
	loader.retrieve(this->length);
	loader.retrieve(this->anchorIndexPos);

	auto t = Locator<BlockIDTextures>::get();
	this->anchorTex = t->getBlockTextureID("crane_anchor.dds");
	this->shaftTex = t->getBlockTextureID("crane_shaft.dds");
	this->supportTex = t->getBlockTextureID("crane_support.dds");

	return true;
}

void RailCrane::appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	glm::vec2 d = ACTIVITY::GETDIRECTION(this->orientation);
	glm::vec2 pos = this->getMovingOrigin(gameState);
	float size = static_cast<float>(1 + this->length);
	if (this->orientation == ACTIVITY::DIR::UP || this->orientation == ACTIVITY::DIR::RIGHT) {
		renderInfo.selectionRenderInfo.addBox(pos, pos + size * d + glm::vec2(1), color);
	}
	else {
		renderInfo.selectionRenderInfo.addBox(pos + glm::vec2(1), pos + size * d, color);
	}
}

void RailCrane::appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	glm::vec2 orientationDir = ACTIVITY::GETDIRECTION(this->orientation);
	glm::vec2 moveableOrigin = this->getMovingOrigin(gameState);

	auto pos = moveableOrigin;

	staticWorldRenderInfo.addBlockWithShadow(pos, this->supportTex);

	for (int32_t i = 0; i < this->length; i++) {
		pos += orientationDir;
		staticWorldRenderInfo.addBlockWithoutShadow(pos, this->shaftTex);
	}

	pos += orientationDir;
	staticWorldRenderInfo.addBlockWithShadow(pos, this->supportTex);

	float activityScale = getActivityScale(gameState.tick);

	glm::vec2 activityDir;
	pos = moveableOrigin + static_cast<float>(1 + this->anchorIndexPos) * orientationDir;
	if (this->active) {
		switch (this->anchorDirection) {
			case RAILCRANE::DIR::LEFT:
				activityDir = -ACTIVITY::GETDIRECTION(this->orientation);
				pos += orientationDir;
				pos += activityScale * activityDir;
				break;
			case RAILCRANE::DIR::RIGHT:
				activityDir = ACTIVITY::GETDIRECTION(this->orientation);
				pos -= orientationDir;
				pos += activityScale * activityDir;
				break;
			default:
				break;
		}
	}
	staticWorldRenderInfo.addBlockWithShadow(pos, this->anchorTex);
}
