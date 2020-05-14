#include "common.h"

#include "RailCrane.h"
#include "BlockIDTextures.h"
#include "GameState.h"
#include "StaticWorldRenderInfo.h"
#include "RenderInfo.h"
#include "ActivityIgnoringGroup.h"
#include "Anchor.h"

void RailCrane::modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) {
}

std::stringstream& RailCrane::getMembers(std::stringstream& out) {
	// TODO: insert return statement here
	return out;
}

RailCrane::RailCrane(Handle self, GameState& gameState, glm::ivec2 pos, bool leavetraces) :
	Grouper(self, pos) {
	length = 6;
	orientation = MOVEABLE::DIR::RIGHT;
	anchorDirection = RAILCRANE::DIR::STATIONARY;
	anchorIndexPos = 0;

	if (leavetraces) {
		fillTracesLocalForced(gameState);
	}

	auto t = Locator<BlockIDTextures>::getService();
	anchorTex = t->getBlockTextureID("crane_anchor.dds");
	shaftTex = t->getBlockTextureID("crane_shaft.dds");
	supportTex = t->getBlockTextureID("crane_support.dds");
}

RailCrane::RailCrane() {
}

RailCrane::~RailCrane() {
}

void RailCrane::rotateForcedLocal(glm::ivec2 center, MOVEABLE::ROT rotation) {
	auto d = origin - center;
	switch (rotation) {
		case MOVEABLE::ROT::CLOCKWISE:
			d = glm::ivec2(d.y, -d.x - 1);
			orientation = static_cast<MOVEABLE::DIR>(static_cast<int>(orientation + 1) % 4);
			break;
		case MOVEABLE::ROT::COUNTERCLOCKWISE:
			orientation = static_cast<MOVEABLE::DIR>(static_cast<int>(orientation + 3) % 4);
			d = glm::ivec2(-d.y - 1, d.x);
			break;
		default:
			break;
	}
	origin = center + d;
}

bool RailCrane::canActivityLocal(GameState& gameState, int type) {
	switch (type) {
		case RAILCRANE::LEFT:
			{
				if (anchorIndexPos == 0) {
					return false;
				}
				if (child.isNotNull()) {
					return child.get()->canMoveUp(gameState, MOVEABLE::REV_DIR[orientation]);
				}
				return true;
				break;
			}
		case RAILCRANE::RIGHT:
			{
				if (anchorIndexPos == length - 1) {
					return false;
				}
				if (child.isNotNull()) {
					return child.get()->canMoveUp(gameState, orientation);
				}
				return true;
				break;
			}
		default:
			break;
	}
	return false;
}

void RailCrane::applyActivityLocalForced(GameState& gameState, int type, int pace) {
	Activity::applyActivityLocalForced(gameState, type, pace);
	switch (type) {
		case RAILCRANE::LEFT:
			if (child.isNotNull()) {
				child.get()->applyMoveUpForced(gameState, MOVEABLE::REV_DIR[orientation], pace);
			}
			anchorDirection = RAILCRANE::LEFT;
			anchorIndexPos--;
			break;
		case RAILCRANE::RIGHT:
			if (child.isNotNull()) {
				child.get()->applyMoveUpForced(gameState, orientation, pace);
			}
			anchorDirection = RAILCRANE::RIGHT;
			anchorIndexPos++;
			break;
		default:
			break;
	}
}

bool RailCrane::canMoveLocal(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) {
	auto d = MOVEABLE::DIRECTION[orientation];
	auto pos = origin + MOVEABLE::DIRECTION[dir];
	return !gameState.staticWorld.isOccupied(pos, ignore)
		&& !gameState.staticWorld.isOccupied(pos + d * (1 + length), ignore);
}

bool RailCrane::canFillTracesLocal(GameState& gameState) {
	return true;
}

void RailCrane::fillTracesLocalForced(GameState& gameState) {
	auto dir = MOVEABLE::DIRECTION[orientation];
	gameState.staticWorld.leaveTrace(origin, selfHandle);
	gameState.staticWorld.leaveTrace(origin + dir * (1 + length), selfHandle);
}

void RailCrane::removeTracesLocalForced(GameState& gameState) {
	auto dir = MOVEABLE::DIRECTION[orientation];
	gameState.staticWorld.removeTraceForced(origin);
	gameState.staticWorld.removeTraceForced(origin + dir * (1 + length));
}

void RailCrane::removeActivityTracesLocal(GameState& gameState) {
}

void RailCrane::leaveActivityTracesLocal(GameState& gameState) {
}

void RailCrane::removeMoveableTracesLocal(GameState& gameState) {
	auto d = MOVEABLE::DIRECTION[orientation];
	auto pos = origin + MOVEABLE::DIRECTION[MOVEABLE::REV_DIR[movementDirection]];
	gameState.staticWorld.removeTraceFilter(pos, selfHandle);
	gameState.staticWorld.removeTraceFilter(pos + d * (1 + length), selfHandle);
}

void RailCrane::leaveMoveableTracesLocal(GameState& gameState) {
	auto d = MOVEABLE::DIRECTION[orientation];
	auto pos = origin + MOVEABLE::DIRECTION[movementDirection];
	gameState.staticWorld.leaveTrace(pos, selfHandle);
	gameState.staticWorld.leaveTrace(pos + d * (1 + length), selfHandle);
}

ACTIVITY::TYPE RailCrane::getType() {
	return ACTIVITY::TYPE::RAILCRANE;
}

void RailCrane::save(Saver& saver) {
	Grouper::save(saver);
	saver.store(orientation);
	saver.store(anchorDirection);
	saver.store(length);
	saver.store(anchorIndexPos);
}

bool RailCrane::load(Loader& loader) {
	Grouper::load(loader);
	loader.retrieve(orientation);
	loader.retrieve(anchorDirection);
	loader.retrieve(length);
	loader.retrieve(anchorIndexPos);

	auto t = Locator<BlockIDTextures>::getService();
	anchorTex = t->getBlockTextureID("crane_anchor.dds");
	shaftTex = t->getBlockTextureID("crane_shaft.dds");
	supportTex = t->getBlockTextureID("crane_support.dds");

	return true;
}

void RailCrane::appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	glm::vec2 d = MOVEABLE::DIRECTION[orientation];
	glm::vec2 pos = getMovingOrigin(gameState);
	float size = static_cast<float>(1 + length);
	if (orientation == MOVEABLE::DIR::UP || orientation == MOVEABLE::DIR::RIGHT) {
		renderInfo.selectionRenderInfo.addBox(pos, pos + size * d + glm::vec2(1));
	}
	else {
		renderInfo.selectionRenderInfo.addBox(pos + glm::vec2(1), pos + size * d);
	}
}

void RailCrane::appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	glm::vec2 orientationDir = MOVEABLE::DIRECTION[orientation];
	glm::vec2 moveableOrigin = getMovingOrigin(gameState);

	auto pos = moveableOrigin;

	staticWorldRenderInfo.addBlockWithShadow(pos, supportTex);

	for (int i = 0; i < length; i++) {
		pos += orientationDir;
		staticWorldRenderInfo.addBlockWithoutShadow(pos, shaftTex);
	}

	pos += orientationDir;
	staticWorldRenderInfo.addBlockWithShadow(pos, supportTex);

	float activityScale = getActivityScale(gameState.tick);

	glm::vec2 activityDir;
	pos = moveableOrigin + static_cast<float>(1 + anchorIndexPos) * orientationDir;
	if (active) {
		switch (anchorDirection) {
			case RAILCRANE::LEFT:
				activityDir = -MOVEABLE::DIRECTION[orientation];
				pos += orientationDir;
				pos += activityScale * activityDir;
				break;
			case RAILCRANE::RIGHT:
				activityDir = MOVEABLE::DIRECTION[orientation];
				pos -= orientationDir;
				pos += activityScale * activityDir;
				break;
			default:
				break;
		}
	}
	staticWorldRenderInfo.addBlockWithShadow(pos, anchorTex);
}
