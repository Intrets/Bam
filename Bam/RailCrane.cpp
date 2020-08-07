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

void RailCrane::modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) {
}

std::stringstream& RailCrane::getMembers(std::stringstream& out) {
	// TODO: insert return statement here
	return out;
}

RailCrane::RailCrane(Handle self, GameState& gameState, glm::ivec2 pos, bool leavetraces) :
	Grouper(self, pos) {
	this->length = 6;
	this->orientation = Activity::DIR::RIGHT;
	this->anchorDirection = RAILCRANE::DIR::STATIONARY;
	this->anchorIndexPos = 0;

	if (leavetraces) {
		fillTracesLocalForced(gameState);
	}

	auto t = Locator<BlockIDTextures>::get();
	this->anchorTex = t->getBlockTextureID("crane_anchor.dds");
	this->shaftTex = t->getBlockTextureID("crane_shaft.dds");
	this->supportTex = t->getBlockTextureID("crane_support.dds");
}

void RailCrane::rotateForcedLocal(glm::ivec2 center, Activity::ROT rotation) {
	auto d = this->origin - center;
	switch (rotation) {
		case Activity::ROT::CLOCKWISE:
			d = glm::ivec2(d.y, -d.x - 1);
			this->orientation = static_cast<Activity::DIR>(static_cast<int32_t>(this->orientation + 1) % 4);
			break;
		case Activity::ROT::COUNTERCLOCKWISE:
			this->orientation = static_cast<Activity::DIR>(static_cast<int32_t>(this->orientation + 3) % 4);
			d = glm::ivec2(-d.y - 1, d.x);
			break;
		default:
			break;
	}
	this->origin = center + d;
}

bool RailCrane::canActivityLocal(GameState& gameState, int32_t type) {
	switch (type) {
		case RAILCRANE::LEFT:
			{
				if (this->anchorIndexPos == 0) {
					return false;
				}
				if (this->child.isNotNull()) {
					return this->child.get()->canMoveUp(gameState, Activity::REV_DIR(this->orientation));
				}
				return true;
				break;
			}
		case RAILCRANE::RIGHT:
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
	Activity::applyActivityLocalForced(gameState, type, pace);
	switch (type) {
		case RAILCRANE::LEFT:
			if (this->child.isNotNull()) {
				this->child.get()->applyMoveUpForced(gameState, Activity::REV_DIR(this->orientation), pace);
			}
			this->anchorDirection = RAILCRANE::LEFT;
			this->anchorIndexPos--;
			break;
		case RAILCRANE::RIGHT:
			if (this->child.isNotNull()) {
				this->child.get()->applyMoveUpForced(gameState, this->orientation, pace);
			}
			this->anchorDirection = RAILCRANE::RIGHT;
			this->anchorIndexPos++;
			break;
		default:
			break;
	}
}

bool RailCrane::canMoveLocal(GameState& gameState, Activity::DIR dir, ActivityIgnoringGroup& ignore) {
	auto d = Activity::getDirection(this->orientation);
	auto pos = this->origin + Activity::getDirection(dir);
	return !gameState.staticWorld.isOccupied(pos, ignore)
		&& !gameState.staticWorld.isOccupied(pos + d * (1 + this->length), ignore);
}

bool RailCrane::canFillTracesLocal(GameState& gameState) {
	auto dir = Activity::getDirection(this->orientation);
	return !gameState.staticWorld.isOccupied(this->origin) 
		&& !gameState.staticWorld.isOccupied(this->origin + dir * (1 + this->length));
}

void RailCrane::fillTracesLocalForced(GameState& gameState) {
	auto dir = Activity::getDirection(this->orientation);
	gameState.staticWorld.leaveTrace(this->origin, this->selfHandle);
	gameState.staticWorld.leaveTrace(this->origin + dir * (1 + this->length), this->selfHandle);
}

void RailCrane::removeTracesLocalForced(GameState& gameState) {
	auto dir = Activity::getDirection(this->orientation);
	gameState.staticWorld.removeTraceForced(this->origin);
	gameState.staticWorld.removeTraceForced(this->origin + dir * (1 + this->length));
}

void RailCrane::removeActivityTracesLocal(GameState& gameState) {
}

void RailCrane::leaveActivityTracesLocal(GameState& gameState) {
}

void RailCrane::removeMoveableTracesLocal(GameState& gameState) {
	auto d = Activity::getDirection(this->orientation);
	auto pos = this->origin + Activity::getDirection(Activity::REV_DIR(this->movementDirection));
	gameState.staticWorld.removeTraceFilter(pos, this->selfHandle);
	gameState.staticWorld.removeTraceFilter(pos + d * (1 + this->length), this->selfHandle);
}

void RailCrane::leaveMoveableTracesLocal(GameState& gameState) {
	auto d = Activity::getDirection(this->orientation);
	auto pos = this->origin + Activity::getDirection(this->movementDirection);
	gameState.staticWorld.leaveTrace(pos, this->selfHandle);
	gameState.staticWorld.leaveTrace(pos + d * (1 + this->length), this->selfHandle);
}

Activity::TYPE RailCrane::getType() {
	return Activity::TYPE::RAILCRANE;
}

void RailCrane::save(Saver& saver) {
	Grouper::save(saver);
	saver.store(this->orientation);
	saver.store(this->anchorDirection);
	saver.store(this->length);
	saver.store(this->anchorIndexPos);
}

bool RailCrane::load(Loader& loader) {
	Grouper::load(loader);
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

void RailCrane::appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	glm::vec2 d = Activity::getDirection(this->orientation);
	glm::vec2 pos = getMovingOrigin(gameState);
	float size = static_cast<float>(1 + this->length);
	if (this->orientation == Activity::DIR::UP || this->orientation == Activity::DIR::RIGHT) {
		renderInfo.selectionRenderInfo.addBox(pos, pos + size * d + glm::vec2(1));
	}
	else {
		renderInfo.selectionRenderInfo.addBox(pos + glm::vec2(1), pos + size * d);
	}
}

void RailCrane::appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	glm::vec2 orientationDir = Activity::getDirection(this->orientation);
	glm::vec2 moveableOrigin = getMovingOrigin(gameState);

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
			case RAILCRANE::LEFT:
				activityDir = -Activity::getDirection(this->orientation);
				pos += orientationDir;
				pos += activityScale * activityDir;
				break;
			case RAILCRANE::RIGHT:
				activityDir = Activity::getDirection(this->orientation);
				pos -= orientationDir;
				pos += activityScale * activityDir;
				break;
			default:
				break;
		}
	}
	staticWorldRenderInfo.addBlockWithShadow(pos, this->anchorTex);
}
