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

int32_t RailCrane::generateActivityPace(GameState& gameState) const {
	return this->headBlock.getBlock().getSmallRand(gameState);
}

int32_t RailCrane::getMaxLength() const {
	return this->shaftBlock.getBlock().getVal();
}

RailCrane::RailCrane(Handle self, GameState& gameState, glm::ivec2 pos) :
	SingleGrouper(self, pos) {
	this->anchorDirection = RAILCRANE::DIR::STATIONARY;
	this->anchorIndexPos = 0;

	this->shaftBlock = ShapedBlock("diorite", "shaft_stencil", ACTIVITY::DIR::RIGHT);
	this->baseBlock = ShapedBlock("diorite", "crane_base_block_stencil", ACTIVITY::DIR::RIGHT);
	this->headBlock = ShapedBlock("diorite", "crane_head_block_stencil", ACTIVITY::DIR::RIGHT);
}

void RailCrane::rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) {
	this->Activity::rotateForcedLocal(center, rotation);

	this->shaftBlock.rotate(rotation);
	this->headBlock.rotate(rotation);

	auto d = this->origin - center;
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

bool RailCrane::canActivityLocal(GameState& gameState, int32_t type) {
	switch (static_cast<RAILCRANE::DIR>(type)) {
		case RAILCRANE::DIR::LEFT:
			{
				if (this->anchorIndexPos == 0) {
					return false;
				}
				if (this->child.isNotNull()) {
					return this->child.get()->canMoveUp(gameState, ACTIVITY::FLIP(this->activityRotation));
				}
				return true;
				break;
			}
		case RAILCRANE::DIR::RIGHT:
			{
				if (this->anchorIndexPos == this->getMaxLength() - 1) {
					return false;
				}
				if (this->child.isNotNull()) {
					return this->child.get()->canMoveUp(gameState, this->activityRotation);
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
	pace = this->headBlock.getBlock().getSmallRand(gameState);

	this->Activity::applyActivityLocalForced(gameState, type, pace);

	switch (static_cast<RAILCRANE::DIR>(type)) {
		case RAILCRANE::DIR::LEFT:
			if (this->child.isNotNull()) {
				this->child.get()->applyMoveUpForced(gameState, ACTIVITY::FLIP(this->activityRotation), pace);
			}
			this->anchorDirection = RAILCRANE::DIR::LEFT;
			this->anchorIndexPos--;
			break;
		case RAILCRANE::DIR::RIGHT:
			if (this->child.isNotNull()) {
				this->child.get()->applyMoveUpForced(gameState, this->activityRotation, pace);
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
			this->anchorIndexPos = glm::min(this->anchorIndexPos + 1, this->getMaxLength() - 1);
			break;
		case RAILCRANE::DIR::STATIONARY:
			break;
		default:
			break;
	}
}

bool RailCrane::canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) {
	auto d = ACTIVITY::GETDIRECTION(this->activityRotation);
	auto pos = this->origin + ACTIVITY::GETDIRECTION(dir) + ACTIVITY::GETDIRECTION(ACTIVITY::COUNTERWISEROT(this->activityRotation));
	return !gameState.staticWorld.isOccupied(pos, ignore)
		&& !gameState.staticWorld.isOccupied(pos + d * (this->getMaxLength() - 1), ignore);
}

bool RailCrane::canFillTracesLocal(GameState& gameState) {
	auto dir = ACTIVITY::GETDIRECTION(this->activityRotation);
	auto pos = this->origin + ACTIVITY::GETDIRECTION(ACTIVITY::COUNTERWISEROT(this->activityRotation));
	return !gameState.staticWorld.isOccupied(pos)
		&& !gameState.staticWorld.isOccupied(pos + dir * (this->getMaxLength() - 1));
}

void RailCrane::fillTracesLocalForced(GameState& gameState) {
	this->Activity::fillTracesLocalForced(gameState);
	auto dir = ACTIVITY::GETDIRECTION(this->activityRotation);
	auto pos = this->origin + ACTIVITY::GETDIRECTION(ACTIVITY::COUNTERWISEROT(this->activityRotation));
	gameState.staticWorld.leaveTrace(pos, this);
	gameState.staticWorld.leaveTrace(pos + dir * (this->getMaxLength() - 1), this);
}

void RailCrane::removeTracesLocalForced(GameState& gameState) {
	this->Activity::removeTracesLocalForced(gameState);
	auto dir = ACTIVITY::GETDIRECTION(this->activityRotation);
	auto pos = this->origin + ACTIVITY::GETDIRECTION(ACTIVITY::COUNTERWISEROT(this->activityRotation));
	gameState.staticWorld.removeTraceForced(pos);
	gameState.staticWorld.removeTraceForced(pos + dir * (this->getMaxLength() - 1));
}

void RailCrane::preMoveableStopLocal(GameState& gameState) {
	auto d = ACTIVITY::GETDIRECTION(this->activityRotation);
	auto pos = this->origin + ACTIVITY::GETDIRECTION(ACTIVITY::FLIP(this->movementDirection)) + ACTIVITY::GETDIRECTION(ACTIVITY::COUNTERWISEROT(this->activityRotation));
	gameState.staticWorld.removeTraceFilter(pos, this);
	gameState.staticWorld.removeTraceFilter(pos + d * (this->getMaxLength() - 1), this);
}

void RailCrane::postMoveableStartLocal(GameState& gameState) {
	auto d = ACTIVITY::GETDIRECTION(this->activityRotation);
	auto pos = this->origin + ACTIVITY::GETDIRECTION(this->movementDirection) + ACTIVITY::GETDIRECTION(ACTIVITY::COUNTERWISEROT(this->activityRotation));
	gameState.staticWorld.leaveTrace(pos, this);
	gameState.staticWorld.leaveTrace(pos + d * (this->getMaxLength() - 1), this);
}

ACTIVITY::TYPE RailCrane::getType() {
	return ACTIVITY::TYPE::RAILCRANE;
}

void RailCrane::save(Saver& saver) {
	this->SingleGrouper::save(saver);
	this->headBlock.save(saver);
	this->shaftBlock.save(saver);

	saver.store(this->anchorDirection);
	saver.store(this->anchorIndexPos);
}

bool RailCrane::load(Loader& loader) {
	this->SingleGrouper::load(loader);
	this->headBlock.load(loader);
	this->shaftBlock.load(loader);

	loader.retrieve(this->anchorDirection);
	loader.retrieve(this->anchorIndexPos);

	return true;
}

void RailCrane::appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	glm::vec2 d = ACTIVITY::GETDIRECTION(this->activityRotation);
	glm::vec2 pos = this->getMovingOrigin(gameState);
	glm::vec2 d2 = ACTIVITY::GETDIRECTION(ACTIVITY::COUNTERWISEROT(this->activityRotation));
	float size = static_cast<float>(this->getMaxLength() - 1);
	switch (this->activityRotation) {
		case ACTIVITY::DIR::DOWN:
			renderInfo.selectionRenderInfo.addBox(pos + glm::vec2(0.0f, 1.0f), pos + size * d + glm::vec2(1.0f, 0.0f) + d2, color);
			break;
		case ACTIVITY::DIR::LEFT:
			renderInfo.selectionRenderInfo.addBox(pos + glm::vec2(1.0f, 1.0f), pos + size * d + d2, color);
			break;
		case ACTIVITY::DIR::UP:
			renderInfo.selectionRenderInfo.addBox(pos + glm::vec2(1.0f, 0.0f), pos + size * d + glm::vec2(0.0f, 1.0f) + d2, color);
			break;
		case ACTIVITY::DIR::RIGHT:
			renderInfo.selectionRenderInfo.addBox(pos, pos + size * d + glm::vec2(1.0f, 1.0f) + d2, color);
			break;
		default:
			break;
	}
}

void RailCrane::appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	glm::vec2 orientationDir = ACTIVITY::GETDIRECTION(this->activityRotation);
	glm::vec2 moveableOrigin = this->getMovingOrigin(gameState);

	glm::vec2 d2 = ACTIVITY::GETDIRECTION(ACTIVITY::COUNTERWISEROT(this->activityRotation));

	auto pos = moveableOrigin;

	staticWorldRenderInfo.addBlock(pos + d2, this->baseBlock);

	for (int32_t i = 0; i < this->getMaxLength(); i++) {
		staticWorldRenderInfo.addBlockBack(pos, this->shaftBlock);
		pos += orientationDir;
	}

	pos -= orientationDir;

	staticWorldRenderInfo.addBlock(pos + d2, this->baseBlock);

	float activityScale = this->getActivityScale(gameState.tick);

	glm::vec2 activityDir;
	pos = moveableOrigin + static_cast<float>(this->anchorIndexPos) * orientationDir;
	if (this->active) {
		switch (this->anchorDirection) {
			case RAILCRANE::DIR::LEFT:
				activityDir = -ACTIVITY::GETDIRECTION(this->activityRotation);
				pos += orientationDir;
				pos += activityScale * activityDir;
				break;
			case RAILCRANE::DIR::RIGHT:
				activityDir = ACTIVITY::GETDIRECTION(this->activityRotation);
				pos -= orientationDir;
				pos += activityScale * activityDir;
				break;
			default:
				break;
		}
	}
	staticWorldRenderInfo.addBlockFront(pos, this->headBlock);
}
