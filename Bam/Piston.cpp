#include "common.h"
#include "Piston.h"

#include "Loader.h"
#include "Saver.h"
#include "GameState.h"
#include "StaticWorldRenderInfo.h"
#include "ActivityIgnoringGroup.h"
#include "BlockIDTextures.h"
#include "RenderInfo.h"
#include "Anchor.h"

Piston::Piston(Handle self, glm::ivec2 pos, ACTIVITY::DIR dir) :
	SingleGrouper(self, pos),
	length(0) {
	this->activityRotation = ACTIVITY::DIR::RIGHT;
	this->shaftBlock = ShapedBlock("iron_ore", "shaft_stencil", ACTIVITY::DIR::RIGHT);
	this->baseBlock = ShapedBlock("iron_ore", "piston_body_stencil", ACTIVITY::DIR::RIGHT);
	this->headBlock = ShapedBlock("iron_ore", "piston_stencil", ACTIVITY::DIR::RIGHT);
}

ACTIVITY::TYPE Piston::getType() {
	return ACTIVITY::TYPE::PISTON;
}

bool Piston::canActivityLocal(GameState& gameState, int32_t type) {
	auto headDirection = ACTIVITY::GETDIRECTION(this->activityRotation);
	switch (static_cast<PISTON::DIR>(type)) {
		case PISTON::DIR::EXTEND:
			{
				if (this->length >= this->shaftBlock.getBlock().getVal()) {

					return false;
				}
				if (this->child.isNotNull()) {
					return this->child.get()->canMoveUp(gameState, this->activityRotation);
				}
				else {
					auto next = this->origin + (this->length + 1) * headDirection;
					return !gameState.staticWorld.isOccupied(next);
				}
			}
			break;
		case PISTON::DIR::RETRACT:
			{
				if (this->length == 0) {
					return false;
				}
				if (this->child.isNull()) {
					return true;
				}
				ActivityIgnoringGroup ignoring{ this->getSortedHandles() };
				return this->child.get()->canMoveUp(gameState, ACTIVITY::FLIP(this->activityRotation), ignoring);
			}
			break;
		default:
			return false;
			break;
	}
}

bool Piston::canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) {
	glm::ivec2 headDirection = ACTIVITY::GETDIRECTION(this->activityRotation);
	glm::ivec2 moveDirection = ACTIVITY::GETDIRECTION(dir);

	int32_t d = idot(moveDirection, headDirection);
	auto ori = this->origin + moveDirection;
	bool blocked = false;
	if (d == 0) {
		for (int32_t i = 0; i <= this->length; i++) {
			auto p = ori + i * headDirection;
			if (gameState.staticWorld.isOccupied(p, ignore)) {
				blocked = true;
				break;
			}
		}
	}
	else if (d == 1) {
		auto p = ori + this->length * headDirection;
		if (gameState.staticWorld.isOccupied(p, ignore)) {
			blocked = true;
		}
	}
	else {
		auto p = ori;
		if (gameState.staticWorld.isOccupied(p, ignore)) {
			blocked = true;
		}
	}
	return !blocked;
}

void Piston::appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	glm::vec2 headDirection = ACTIVITY::GETDIRECTION(this->activityRotation);
	glm::vec2 moveDirection = ACTIVITY::GETDIRECTION(this->movementDirection);
	glm::vec2 ori = this->getMovingOrigin(gameState);

	float size = static_cast<float>(this->length);

	switch (static_cast<PISTON::DIR>(this->activityType)) {
		case PISTON::DIR::EXTEND:
			size += this->getActivityScaleForced(gameState.tick);
			break;
		case PISTON::DIR::RETRACT:
			size -= this->getActivityScaleForced(gameState.tick);
			break;
		default:
			break;
	}

	glm::vec2 base = ori;
	glm::vec2 head = ori + headDirection * size + glm::vec2(1.0f);

	switch (this->activityRotation) {
		case ACTIVITY::DIR::DOWN:
		case ACTIVITY::DIR::LEFT:
			base += glm::vec2(1.0f, 1.0f);
			head -= glm::vec2(1.0f, 1.0f);
			break;
		case ACTIVITY::DIR::UP:
		case ACTIVITY::DIR::RIGHT:
			break;
		default:
			break;
	}

	renderInfo.selectionRenderInfo.addBox(base, head, color);
}

void Piston::appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	glm::vec2 headDirection = ACTIVITY::GETDIRECTION(this->activityRotation);
	glm::vec2 moveDirection = ACTIVITY::GETDIRECTION(this->movementDirection);

	glm::vec2 ori = this->getMovingOrigin(gameState);

	staticWorldRenderInfo.addBlock(ori, this->baseBlock);

	float s = 0;
	int32_t i;
	switch (static_cast<PISTON::DIR>(this->activityType)) {
		case PISTON::DIR::EXTEND:
			s = this->getActivityScaleForced(gameState.tick);
			i = 0;
			break;
		case PISTON::DIR::RETRACT:
			s = -this->getActivityScaleForced(gameState.tick);
			i = 1;
			break;
		default:
			i = 0;
			break;
	}

	for (; i < this->length; i++) {
		auto p = ori + (s + static_cast<float>(i)) * headDirection;
		staticWorldRenderInfo.addBlock(p, this->shaftBlock);
	}

	float size = s + static_cast<float>(this->length);

	auto p = ori + size * headDirection;
	staticWorldRenderInfo.addBlock(p, this->headBlock);
}

void Piston::preActivityStopLocal(GameState& gameState) {
	switch (static_cast<PISTON::DIR>(this->activityType)) {
		case PISTON::DIR::EXTEND:
			this->length++;
			break;
		case PISTON::DIR::RETRACT:
			this->length--;
			break;
		default:
			break;
	}
}

void Piston::preMoveableStopLocal(GameState& gameState) {
	glm::ivec2 headDirection = ACTIVITY::GETDIRECTION(this->activityRotation);
	int32_t d = idot(GETDIRECTION(this->movementDirection), headDirection);
	auto ori = this->origin - GETDIRECTION(this->movementDirection);
	if (d == 0) {
		for (int32_t i = 0; i <= this->length; i++) {
			auto p = ori + i * headDirection;
			gameState.staticWorld.removeTraceFilter(p, this);
		}
	}
	else if (d == -1) {
		auto p = ori + this->length * headDirection;
		gameState.staticWorld.removeTraceFilter(p, this);
	}
	else {
		auto p = ori;
		gameState.staticWorld.removeTraceFilter(p, this);
	}
}

void Piston::postMoveableStartLocal(GameState& gameState) {
	glm::ivec2 headDirection = ACTIVITY::GETDIRECTION(this->activityRotation);
	int32_t d = idot(GETDIRECTION(this->movementDirection), headDirection);
	auto ori = this->origin + GETDIRECTION(this->movementDirection);
	if (d == 0) {
		for (int32_t i = 0; i <= this->length; i++) {
			auto p = ori + i * headDirection;
			gameState.staticWorld.leaveTrace(p, this);
		}
	}
	else if (d == 1) {
		auto p = ori + this->length * headDirection;
		gameState.staticWorld.leaveTrace(p, this);
	}
	else {
		auto p = ori;
		gameState.staticWorld.leaveTrace(p, this);
	}
}

void Piston::save(Saver& saver) {
	this->SingleGrouper::save(saver);
	this->headBlock.save(saver);
	this->shaftBlock.save(saver);
	saver.store(this->length);
}

bool Piston::load(Loader& loader) {
	this->SingleGrouper::load(loader);
	this->headBlock.load(loader);
	this->shaftBlock.load(loader);
	loader.retrieve(this->length);

	return true;
}

void Piston::rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) {
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

void Piston::fillTracesLocalForced(GameState& gameState) {
	this->Activity::fillTracesLocalForced(gameState);
	glm::ivec2 pos = this->origin;
	for (int32_t i = 0; i < this->length + 1; i++) {
		gameState.staticWorld.leaveTrace(pos, this);
		pos += ACTIVITY::GETDIRECTION(this->activityRotation);
	}
}

void Piston::removeTracesLocalForced(GameState& gameState) {
	this->Activity::removeTracesLocalForced(gameState);
	glm::ivec2 pos = this->origin;
	for (int32_t i = 0; i < this->length + 1; i++) {
		gameState.staticWorld.removeTraceForced(pos);
		pos += ACTIVITY::GETDIRECTION(this->activityRotation);
	}
}

void Piston::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	pace = this->shaftBlock.getBlock().getSmallRand(gameState);

	this->Activity::applyActivityLocalForced(gameState, type, pace);
	glm::ivec2 headDirection = ACTIVITY::GETDIRECTION(this->activityRotation);
	switch (static_cast<PISTON::DIR>(type)) {
		case PISTON::DIR::EXTEND:
			{
				auto next = this->origin + (this->length + 1) * headDirection;
				gameState.staticWorld.leaveTrace(next, this);
				if (this->child.isNotNull()) {
					this->child.get()->applyMoveUpForced(gameState, this->activityRotation, pace);
				}
			}
			break;
		case PISTON::DIR::RETRACT:
			{
				auto headpos = this->origin + this->length * headDirection;
				gameState.staticWorld.removeTraceFilter(headpos, this);
				if (this->child.isNotNull()) {
					this->child.get()->applyMoveUpForced(gameState, ACTIVITY::FLIP(this->activityRotation), pace);
				}
			}
			break;
		default:
			break;
	}
}

void Piston::forceChangeActivityState(int32_t type) {
	switch (static_cast<PISTON::DIR>(type)) {
		case PISTON::DIR::STATIONARY:
			break;
		case PISTON::DIR::RETRACT:
			this->length = glm::max(0, this->length - 1);
			break;
		case PISTON::DIR::EXTEND:
			this->length++;
			break;
		default:
			break;
	}
}

bool Piston::canFillTracesLocal(GameState& gameState) {
	glm::ivec2 pos = this->origin;
	for (int32_t i = 0; i <= this->length; i++) {
		if (gameState.staticWorld.isOccupied(pos)) {
			return false;
		}
		pos += ACTIVITY::GETDIRECTION(this->activityRotation);
	}
	return true;
}

