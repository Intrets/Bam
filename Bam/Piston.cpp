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

int32_t Piston::cogTex = 0;
int32_t Piston::ropeTex = 0;
int32_t Piston::headTex = 0;

Piston::Piston(Handle self, glm::ivec2 pos, ACTIVITY::DIR dir) :
	SingleGrouper(self, pos),
	length(0) {
	this->activityRotation = dir;
}

ACTIVITY::TYPE Piston::getType() {
	return ACTIVITY::TYPE::PISTON;
}

bool Piston::canActivityLocal(GameState& gameState, int32_t type) {
	auto headDirection = ACTIVITY::GETDIRECTION(this->activityRotation);
	switch (static_cast<PISTON::DIR>(type)) {
		case PISTON::DIR::EXTEND:
			{
				if (this->length >= this->shaftMaterial.getVal()) {
					return false;
				}
				if (this->child.isNotNull()) {
					return this->child.get()->canMoveUp(gameState, this->activityRotation);
				}
				else {
					auto next = this->origin + (this->length + 2) * headDirection;
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
		for (int32_t i = 0; i <= this->length + 1; i++) {
			auto p = ori + i * headDirection;
			if (gameState.staticWorld.isOccupied(p, ignore)) {
				blocked = true;
				break;
			}
		}
	}
	else if (d == 1) {
		auto p = ori + (this->length + 1) * headDirection;
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

	float size = static_cast<float>(this->length + 1);

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

	staticWorldRenderInfo.addBlockWithShadow(ori, this->cogTex, this->activityRotation);

	for (int32_t i = 1; i <= this->length; i++) {
		auto p = ori + static_cast<float>(i) * headDirection;
		staticWorldRenderInfo.addBlockWithoutShadow(p, this->ropeTex, this->activityRotation);
	}

	float size = static_cast<float>(this->length + 1);

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

	auto p = ori + size * headDirection;
	staticWorldRenderInfo.addBlockWithShadow(p, this->headTex, this->activityRotation);
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
		for (int32_t i = 0; i <= this->length + 1; i++) {
			auto p = ori + i * headDirection;
			gameState.staticWorld.removeTraceFilter(p, this->selfHandle);
		}
	}
	else if (d == -1) {
		auto p = ori + (this->length + 1) * headDirection;
		gameState.staticWorld.removeTraceFilter(p, this->selfHandle);
	}
	else {
		auto p = ori;
		gameState.staticWorld.removeTraceFilter(p, this->selfHandle);
	}
}

void Piston::postMoveableStartLocal(GameState& gameState) {
	glm::ivec2 headDirection = ACTIVITY::GETDIRECTION(this->activityRotation);
	int32_t d = idot(GETDIRECTION(this->movementDirection), headDirection);
	auto ori = this->origin + GETDIRECTION(this->movementDirection);
	if (d == 0) {
		for (int32_t i = 0; i <= this->length + 1; i++) {
			auto p = ori + i * headDirection;
			gameState.staticWorld.leaveTrace(p, this->selfHandle);
		}
	}
	else if (d == 1) {
		auto p = ori + (this->length + 1) * headDirection;
		gameState.staticWorld.leaveTrace(p, this->selfHandle);
	}
	else {
		auto p = ori;
		gameState.staticWorld.leaveTrace(p, this->selfHandle);
	}
}

void Piston::save(Saver& saver) {
	this->SingleGrouper::save(saver);
	saver.store(this->length);
}

bool Piston::load(Loader& loader) {
	this->SingleGrouper::load(loader);
	loader.retrieve(this->length);
	return true;
}

void Piston::rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) {
	this->Activity::rotateForcedLocal(center, rotation);
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
	for (int32_t i = 0; i < this->length + 2; i++) {
		gameState.staticWorld.leaveTrace(pos, this->selfHandle);
		pos += ACTIVITY::GETDIRECTION(this->activityRotation);
	}
}

void Piston::removeTracesLocalForced(GameState& gameState) {
	this->Activity::removeTracesLocalForced(gameState);
	glm::ivec2 pos = this->origin;
	for (int32_t i = 0; i < this->length + 2; i++) {
		gameState.staticWorld.removeTraceForced(pos);
		pos += ACTIVITY::GETDIRECTION(this->activityRotation);
	}
}

void Piston::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {

	pace = this->shaftMaterial.getSmallRand(gameState);

	this->Activity::applyActivityLocalForced(gameState, type, pace);
	glm::ivec2 headDirection = ACTIVITY::GETDIRECTION(this->activityRotation);
	switch (static_cast<PISTON::DIR>(type)) {
		case PISTON::DIR::EXTEND:
			{
				auto next = this->origin + (this->length + 2) * headDirection;
				gameState.staticWorld.leaveTrace(next, this->selfHandle);
				if (this->child.isNotNull()) {
					this->child.get()->applyMoveUpForced(gameState, this->activityRotation, pace);
				}
			}
			break;
		case PISTON::DIR::RETRACT:
			{
				auto headpos = this->origin + (this->length + 1) * headDirection;
				gameState.staticWorld.removeTraceFilter(headpos, this->selfHandle);
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
	for (int32_t i = 0; i < this->length + 2; i++) {
		if (gameState.staticWorld.isOccupied(pos)) {
			return false;
		}
		pos += ACTIVITY::GETDIRECTION(this->activityRotation);
	}
	return true;
}

