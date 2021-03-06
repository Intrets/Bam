#include "common.h"

#include "Platform.h"
#include "BlockIDTextures.h"
#include "GameState.h"
#include "Anchor.h"
#include "StaticWorldChunk.h"	
#include "Saver.h"
#include "Loader.h"
#include "RenderInfo.h"
#include "WorldBlock.h"

void Platform::calculateBlockedDirections() {
	for (int32_t i = 0; i < 4; i++) {
		this->blockedDirections[i].clear();
	}
	for (int32_t x = 0; x < this->size.x; x++) {
		{
			int32_t y = 0;
			if (this->blocks[x][y].isSolid()) {
				this->blockedDirections[ACTIVITY::DIR::DOWN].push_back(glm::ivec2(x, y - 1));
			}
		}
		{
			int32_t y = size.y - 1;
			if (this->blocks[x][y].isSolid()) {
				this->blockedDirections[ACTIVITY::DIR::UP].push_back(glm::ivec2(x, y + 1));
			}
		}
		for (int32_t y = 0; y < size.y - 1; y++) {
			if (this->blocks[x][y].isSolid() && !this->blocks[x][y + 1].isSolid()) {
				this->blockedDirections[ACTIVITY::DIR::UP].push_back(glm::ivec2(x, y + 1));
			}
			else if (!this->blocks[x][y].isSolid() && this->blocks[x][y + 1].isSolid()) {
				this->blockedDirections[ACTIVITY::DIR::DOWN].push_back(glm::ivec2(x, y));
			}
		}
	}
	for (int32_t y = 0; y < size.y; y++) {
		{
			int32_t x = 0;
			if (this->blocks[x][y].isSolid()) {
				this->blockedDirections[ACTIVITY::DIR::LEFT].push_back(glm::ivec2(x - 1, y));
			}
		}
		{
			int32_t x = size.x - 1;
			if (this->blocks[x][y].isSolid()) {
				this->blockedDirections[ACTIVITY::DIR::RIGHT].push_back(glm::ivec2(x + 1, y));
			}
		}
		for (int32_t x = 0; x < size.x - 1; x++) {
			if (this->blocks[x][y].isSolid() && !this->blocks[x + 1][y].isSolid()) {
				this->blockedDirections[ACTIVITY::DIR::RIGHT].push_back(glm::ivec2(x + 1, y));
			}
			else if (!this->blocks[x][y].isSolid() && this->blocks[x + 1][y].isSolid()) {
				this->blockedDirections[ACTIVITY::DIR::LEFT].push_back(glm::ivec2(x, y));
			}
		}
	}
}

Platform::Platform(Handle self, GameState& gameState, glm::ivec2 _size, glm::ivec2 pos) :
	Activity(self, pos),
	size(_size),
	blocks(this->size[0], std::vector<ShapedBlock>(this->size[1], ShapedBlock())) {
	//int32_t textureID = Locator<BlockIDTextures>::get()->getBlockTextureID("mossy_cobblestone.dds");
	for (int32_t i = 0; i < this->size[0]; i++) {
		for (int32_t j = 0; j < this->size[1]; j++) {
			auto p = origin + glm::ivec2(i, j);
			// REIMPLEMENT: platforms
			//this->blocks[i][j].setID(textureID);
		}
	}
	//this->blocks[0][1].setID(4);
	//this->blocks[1][0].setID(5);

	this->calculateBlockedDirections();
}

void Platform::rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) {
	auto d = origin - center;
	this->size = glm::ivec2(this->size.y, this->size.x);
	auto old = this->blocks;
	this->blocks.clear();
	this->blocks.resize(size.x, std::vector<ShapedBlock>(this->size.y));
	switch (rotation) {
		case ACTIVITY::ROT::CLOCKWISE:
			d = glm::ivec2(d.y, -d.x - this->size.y);
			for (int32_t i = 0; i < this->size.x; i++) {
				for (int32_t j = 0; j < this->size.y; j++) {
					this->blocks[i][j] = old[this->size.y - 1 - j][i];
				}
			}
			this->calculateBlockedDirections();
			break;
		case ACTIVITY::ROT::COUNTERCLOCKWISE:
			d = glm::ivec2(-d.y - this->size.x, d.x);
			for (int32_t i = 0; i < this->size.x; i++) {
				for (int32_t j = 0; j < this->size.y; j++) {
					this->blocks[i][j] = old[j][this->size.x - 1 - i];
				}
			}
			this->calculateBlockedDirections();
			break;
		default:
			break;
	}
	origin = center + d;
}

void Platform::appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	int32_t tick = gameState.tick;
	glm::vec2 v = glm::vec2(origin);
	if (moving) {
		float scale = static_cast<float>(tick - movingTickStart) / movingPace;
		v += scale * glm::vec2(ACTIVITY::GETDIRECTION(movementDirection));
	}
	//renderInfo.selectionRenderInfo.addBox(v, v + glm::vec2(size) - glm::vec2(1.0, 1.0));
	renderInfo.selectionRenderInfo.addBox(v, v + glm::vec2(this->size), color);
	renderInfo.debugRenderInfo.addPoint(v);

}

void Platform::appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	int32_t tick = gameState.tick;
	glm::vec2 v = glm::vec2(origin);
	if (moving) {
		float scale = static_cast<float>(tick - movingTickStart) / movingPace;
		v += scale * glm::vec2(ACTIVITY::GETDIRECTION(movementDirection));
	}
}

ACTIVITY::TYPE Platform::getType() {
	return ACTIVITY::TYPE::PLATFORM;
}

void Platform::impl_getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) {
	members.push_back({ depth, this });
}

bool Platform::canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) {
	glm::ivec2 movedOrigin = origin + ACTIVITY::GETDIRECTION(dir);
	glm::ivec2 p1 = floordiv(movedOrigin, CHUNKSIZE);
	glm::ivec2 p2 = floordiv(movedOrigin + this->size, CHUNKSIZE);

	glm::ivec2 localSize = 1 + p2 - p1;
	std::vector<std::vector<StaticWorldChunk*>> nearChunks(localSize.x, std::vector<StaticWorldChunk*>(localSize.y, nullptr));

	// TODO: move logic of this loop to staticworld?
	for (int32_t i = p1.x; i <= p2.x; i++) {
		for (int32_t j = p1.y; j <= p2.y; j++) {
			nearChunks[i - p1.x][j - p1.y] = &gameState.staticWorld.getChunkByIndex(i, j);
		}
	}
	bool blocked = false;
	for (glm::ivec2& v_ : this->blockedDirections[dir]) {
		glm::ivec2 v = origin + v_;
		//debug->points.push_back(v);
		auto r = floordivmod(v, CHUNKSIZE);
		glm::ivec2 nearCoord = r.first - p1;
		glm::ivec2 chunkCoord = r.second;
		if (nearChunks[nearCoord.x][nearCoord.y]->isOccupied(chunkCoord, ignore)) {
			blocked = true;
			break;
		}
	}
	return !blocked;
}

void Platform::preMoveableStopLocal(GameState& gameState) {
	for (auto& dir : this->blockedDirections[ACTIVITY::FLIP(movementDirection)]) {
		gameState.staticWorld.removeTraceFilter(origin + dir, this);
	}
}

void Platform::postMoveableStartLocal(GameState& gameState) {
	for (auto& dir : this->blockedDirections[movementDirection]) {
		gameState.staticWorld.leaveTrace(origin + dir, this);
	}
}

bool Platform::canActivityLocal(GameState& gameState, int32_t useType) {
	return false;
}

void Platform::save(Saver& saver) {
	this->Activity::save(saver);
	saver.store<glm::ivec2>(this->size);
}

bool Platform::load(Loader& loader) {
	this->Activity::load(loader);
	loader.retrieve<glm::ivec2>(this->size);

	this->blocks = std::vector<std::vector<ShapedBlock>>(this->size[0], std::vector<ShapedBlock>(this->size[1], ShapedBlock()));
	//int32_t textureID = Locator<BlockIDTextures>::get()->getBlockTextureID("mossy_cobblestone.dds");
	for (int32_t i = 0; i < this->size[0]; i++) {
		for (int32_t j = 0; j < this->size[1]; j++) {
			auto p = origin + glm::ivec2(i, j);
			// REIMPLEMENT: setting blocks of platform activity 
			//this->blocks[i][j].setID(textureID);
		}
	}

	this->calculateBlockedDirections();
	return true;
}

void Platform::impl_getTreeMembers(std::vector<Activity*>& members) {
	members.push_back(this);
}

bool Platform::canFillTracesLocal(GameState& gameState) {
	for (int32_t i = 0; i < this->size.x; i++) {
		for (int32_t j = 0; j < this->size.y; j++) {
			auto p = origin + glm::ivec2(i, j);
			if (gameState.staticWorld.isOccupied(p)) {
				return false;
			}
		}
	}
	return true;
}

void Platform::fillTracesLocalForced(GameState& gameState) {
	Activity::fillTracesLocalForced(gameState);
	for (int32_t i = 0; i < this->size.x; i++) {
		for (int32_t j = 0; j < this->size.y; j++) {
			auto p = origin + glm::ivec2(i, j);
			gameState.staticWorld.leaveTrace(p, this);
		}
	}
}

void Platform::removeTracesLocalForced(GameState& gameState) {
	Activity::removeTracesLocalForced(gameState);
	for (int32_t i = 0; i < this->size.x; i++) {
		for (int32_t j = 0; j < this->size.y; j++) {
			auto p = origin + glm::ivec2(i, j);
			gameState.staticWorld.removeTraceForced(p);
		}
	}
}

void Platform::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	this->Activity::applyActivityLocalForced(gameState, type, pace);
}
