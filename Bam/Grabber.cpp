#include "common.h"

#include "Grabber.h"
#include "GameState.h"
#include "StaticWorldRenderInfo.h"
#include "RenderInfo.h"
#include "Saver.h"
#include "Loader.h"
#include "WorldBlock.h"

glm::ivec2 Grabber::getGrabbedOffset() const {
	switch (this->activityRotation) {
		case ACTIVITY::DIR::UP:
			return { 0,1 };
			break;
		case ACTIVITY::DIR::DOWN:
			return { 0,-1 };
			break;
		case ACTIVITY::DIR::RIGHT:
			return { 1,0 };
			break;
		case ACTIVITY::DIR::LEFT:
			return { -1,0 };
			break;
		default:
			return { 0,0 };
			break;
	}
}

glm::ivec2 Grabber::getGrabbedPos() const {
	return this->origin + this->getGrabbedOffset();
}

Grabber::Grabber(Handle self, glm::ivec2 pos) : Activity(self, pos) {
	this->baseBlock = ShapedBlock("diorite", SHAPE::TYPE::GRABBER, ACTIVITY::DIR::RIGHT);
}

void Grabber::rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) {
	this->Activity::rotateForcedLocal(center, rotation);

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

bool Grabber::canActivityLocal(GameState& gameState, int32_t type) {
	if (type == GRABBER::STATE::RELEASE) {
		return this->block.has_value();
	}
	else if (type == GRABBER::STATE::GRAB) {
		return !this->block.has_value() && gameState.staticWorld.getBlockRef(this->getGrabbedPos()).isSolid();
	}
	return false;
}

void Grabber::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	pace = this->baseBlock.getBlock().material.getSmallRand(gameState);

	this->Activity::applyActivityLocalForced(gameState, type, pace);

	if (this->activityType == GRABBER::STATE::RELEASE) {
		gameState.staticWorld.removeTraceForced(this->getGrabbedPos());
		gameState.staticWorld.setBlockForce(this->block.value(), this->getGrabbedPos());
		this->block = std::nullopt;
	}
	else if (this->activityType == GRABBER::STATE::GRAB) {
		auto& b = gameState.staticWorld.getBlockRef(this->getGrabbedPos());
		this->block = b.getShapedBlock();
		b.setBlock(ShapedBlock());
		b.setTrace(this->getHandle());
	}
}

void Grabber::removeTracesLocalForced(GameState& gameState) {
	gameState.staticWorld.removeTraceForced(this->getOrigin());
	if (this->block.has_value()) {
		gameState.staticWorld.removeTraceForced(this->getGrabbedPos());
	}
}

void Grabber::fillTracesLocalForced(GameState& gameState) {
	gameState.staticWorld.leaveTrace(this->getOrigin(), this->getHandle());
	if (this->block.has_value()) {
		gameState.staticWorld.leaveTrace(this->getGrabbedPos(), this->getHandle());
	}
}

ACTIVITY::TYPE Grabber::getType() {
	return ACTIVITY::TYPE::GRABBER;
}

void Grabber::save(Saver& saver) {
	this->Activity::save(saver);
	saver.store(this->block);
}

bool Grabber::load(Loader& loader) {
	this->Activity::load(loader);
	loader.retrieve(this->block);
	return true;
}

void Grabber::appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	glm::vec2 ori = this->getMovingOrigin(gameState);

	staticWorldRenderInfo.addBlock(this->getMovingOrigin(gameState), this->baseBlock);

	if (this->block.has_value()) {
		staticWorldRenderInfo.addBlock(this->getMovingOrigin(gameState) + glm::vec2(this->getGrabbedOffset()), this->block.value());
	}
}

bool Grabber::canMoveLocal(GameState& gameState, ACTIVITY::DIR d, ActivityIgnoringGroup& ignore) {
	auto moveDir = ACTIVITY::GETDIRECTION(d);

	if (this->block.has_value()) {
		auto grabDir = ACTIVITY::GETDIRECTION(this->activityRotation);

		auto s = idot(moveDir, grabDir);

		switch (s) {
			case -1:
				return !gameState.staticWorld.isOccupied(this->getOrigin() + moveDir, ignore);
				break;
			case 0:
				return !gameState.staticWorld.isOccupied(this->getOrigin() + moveDir, ignore) && !gameState.staticWorld.isOccupied(this->getGrabbedPos() + moveDir, ignore);
				break;
			case 1:
				return !gameState.staticWorld.isOccupied(this->getGrabbedPos() + moveDir, ignore);
				break;
			default:
				break;
		}
	}
	else {
		return !gameState.staticWorld.isOccupied(this->getOrigin() + moveDir, ignore);
	}
	return false;
}

bool Grabber::canFillTracesLocal(GameState& gameState) {
	if (this->block.has_value()) {
		return !gameState.staticWorld.isOccupied(this->getOrigin()) && !gameState.staticWorld.isOccupied(this->getGrabbedPos());
	}
	else {
		return !gameState.staticWorld.isOccupied(this->getOrigin());
	}
}

void Grabber::preMoveableStopLocal(GameState& gameState) {
	if (this->block.has_value()) {
		auto moveDir = ACTIVITY::GETDIRECTION(ACTIVITY::FLIP(this->movementDirection));
		auto grabDir = ACTIVITY::GETDIRECTION(this->activityRotation);

		auto d = idot(moveDir, grabDir);

		switch (d) {
			case -1:
				gameState.staticWorld.removeTraceFilter(this->getOrigin() + moveDir, this->getHandle());
				break;
			case 0:
				gameState.staticWorld.removeTraceFilter(this->getOrigin() + moveDir, this->getHandle());
				gameState.staticWorld.removeTraceFilter(this->getGrabbedPos() + moveDir, this->getHandle());
				break;
			case 1:
				gameState.staticWorld.removeTraceFilter(this->getGrabbedPos() + moveDir, this->getHandle());
				break;
			default:
				break;
		}
	}
	else {
		gameState.staticWorld.removeTraceFilter(this->getOrigin() + ACTIVITY::GETDIRECTION(ACTIVITY::FLIP(this->movementDirection)), this->getHandle());
	}
}

void Grabber::postMoveableStartLocal(GameState& gameState) {
	if (this->block.has_value()) {
		auto moveDir = ACTIVITY::GETDIRECTION(this->movementDirection);
		auto grabDir = ACTIVITY::GETDIRECTION(this->activityRotation);

		auto d = idot(moveDir, grabDir);

		switch (d) {
			case -1:
				gameState.staticWorld.leaveTrace(this->getOrigin() + moveDir, this->getHandle());
				break;
			case 0:
				gameState.staticWorld.leaveTrace(this->getOrigin() + moveDir, this->getHandle());
				gameState.staticWorld.leaveTrace(this->getGrabbedPos() + moveDir, this->getHandle());
				break;
			case 1:
				gameState.staticWorld.leaveTrace(this->getGrabbedPos() + moveDir, this->getHandle());
				break;
			default:
				break;
		}
	}
	else {
		gameState.staticWorld.leaveTrace(this->getOrigin() + ACTIVITY::GETDIRECTION(this->movementDirection), this->getHandle());
	}
}

void Grabber::impl_getTreeMembers(std::vector<Activity*>& members) {
	members.push_back(this);
}

void Grabber::impl_getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) {
	members.push_back({ depth, this });
}

void Grabber::appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	if (this->block.has_value()) {
		auto p1 = this->getMovingOrigin(gameState);
		auto p2 = p1 + glm::vec2(this->getGrabbedOffset());

		switch (this->activityRotation) {
			case ACTIVITY::DIR::UP:
			case ACTIVITY::DIR::RIGHT:
				renderInfo.selectionRenderInfo.addBox(p1, p2 + 1.0f, color);
				break;
			case ACTIVITY::DIR::LEFT:
			case ACTIVITY::DIR::DOWN:
				renderInfo.selectionRenderInfo.addBox(p1 + 1.0f, p2, color);
				break;
			default:
				break;
		}
	}
	else {
		auto p = this->getMovingOrigin(gameState);
		renderInfo.selectionRenderInfo.addBox(p, p + 1.0f, color);
	}
}
