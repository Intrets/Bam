#include "common.h"

#include "Platform.h"
#include "BlockIDTextures.h"
#include "GameState.h"
#include "StaticWorldRenderer.h"
#include "Anchor.h"
#include "StaticWorldChunk.h"	
#include "Saver.h"
#include "Loader.h"
#include "RenderInfo.h"

void Platform::calculateBlockedDirections() {
	for (int32_t i = 0; i < 4; i++) {
		this->blockedDirections[i].clear();
	}
	for (int32_t x = 0; x < this->size.x; x++) {
		{
			int32_t y = 0;
			if (this->blocks[x][y].isOccupied()) {
				this->blockedDirections[Activity::DIR::DOWN].push_back(glm::ivec2(x, y - 1));
			}
		}
		{
			int32_t y = size.y - 1;
			if (this->blocks[x][y].isOccupied()) {
				this->blockedDirections[Activity::DIR::UP].push_back(glm::ivec2(x, y + 1));
			}
		}
		for (int32_t y = 0; y < size.y - 1; y++) {
			if (this->blocks[x][y].isOccupied() && !this->blocks[x][y + 1].isOccupied()) {
				this->blockedDirections[Activity::DIR::UP].push_back(glm::ivec2(x, y + 1));
			}
			else if (!this->blocks[x][y].isOccupied() && this->blocks[x][y + 1].isOccupied()) {
				this->blockedDirections[Activity::DIR::DOWN].push_back(glm::ivec2(x, y));
			}
		}
	}
	for (int32_t y = 0; y < size.y; y++) {
		{
			int32_t x = 0;
			if (this->blocks[x][y].isOccupied()) {
				this->blockedDirections[Activity::DIR::LEFT].push_back(glm::ivec2(x - 1, y));
			}
		}
		{
			int32_t x = size.x - 1;
			if (this->blocks[x][y].isOccupied()) {
				this->blockedDirections[Activity::DIR::RIGHT].push_back(glm::ivec2(x + 1, y));
			}
		}
		for (int32_t x = 0; x < size.x - 1; x++) {
			if (this->blocks[x][y].isOccupied() && !this->blocks[x + 1][y].isOccupied()) {
				this->blockedDirections[Activity::DIR::RIGHT].push_back(glm::ivec2(x + 1, y));
			}
			else if (!this->blocks[x][y].isOccupied() && this->blocks[x + 1][y].isOccupied()) {
				this->blockedDirections[Activity::DIR::LEFT].push_back(glm::ivec2(x, y));
			}
		}
	}
}

Platform::Platform(Handle self, GameState& gameState, glm::ivec2 _size, glm::ivec2 pos, bool leaveTraces) :
	Activity(self, pos),
	size(_size),
	blocks(this->size[0], std::vector<Block>(this->size[1], Block(0))) {
	int32_t textureID = Locator<BlockIDTextures>::get()->getBlockTextureID("mossy_cobblestone.dds");
	for (int32_t i = 0; i < this->size[0]; i++) {
		for (int32_t j = 0; j < this->size[1]; j++) {
			auto p = origin + glm::ivec2(i, j);
			this->blocks[i][j].setID(textureID);
		}
	}
	this->blocks[0][1].setID(4);
	this->blocks[1][0].setID(5);
	if (leaveTraces) {
		fillTracesLocalForced(gameState);
	}
	calculateBlockedDirections();
}

void Platform::rotateForcedLocal(glm::ivec2 center, Activity::ROT rotation) {
	auto d = origin - center;
	this->size = glm::ivec2(this->size.y, this->size.x);
	auto old = this->blocks;
	this->blocks.clear();
	this->blocks.resize(size.x, std::vector<Block>(this->size.y));
	switch (rotation) {
		case Activity::ROT::CLOCKWISE:
			d = glm::ivec2(d.y, -d.x - this->size.y);
			for (int32_t i = 0; i < this->size.x; i++) {
				for (int32_t j = 0; j < this->size.y; j++) {
					this->blocks[i][j] = old[this->size.y - 1 - j][i];
				}
			}
			calculateBlockedDirections();
			break;
		case Activity::ROT::COUNTERCLOCKWISE:
			d = glm::ivec2(-d.y - this->size.x, d.x);
			for (int32_t i = 0; i < this->size.x; i++) {
				for (int32_t j = 0; j < this->size.y; j++) {
					this->blocks[i][j] = old[j][this->size.x - 1 - i];
				}
			}
			calculateBlockedDirections();
			break;
		default:
			break;
	}
	origin = center + d;
}

void Platform::appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	int32_t tick = gameState.tick;
	glm::vec2 v = glm::vec2(origin);
	if (moving) {
		float scale = static_cast<float>(tick - movingTickStart) / movingPace;
		v += scale * glm::vec2(getDirection(movementDirection));
	}
	//renderInfo.selectionRenderInfo.addBox(v, v + glm::vec2(size) - glm::vec2(1.0, 1.0));
	renderInfo.selectionRenderInfo.addBox(v, v + glm::vec2(this->size), color);
	renderInfo.debugRenderInfo.addPoint(v);

}

void Platform::appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	int32_t tick = gameState.tick;
	glm::vec2 v = glm::vec2(origin);
	if (moving) {
		float scale = static_cast<float>(tick - movingTickStart) / movingPace;
		v += scale * glm::vec2(getDirection(movementDirection));
	}
	for (int32_t x = 0; x < this->size.x; x++) {
		for (int32_t y = 0; y < this->size.y; y++) {
			// TODO: remove debugging textures 
			if (this->blocks[x][y].isOccupied()) {
				glm::vec2 p(x, y);
				p += v;
				staticWorldRenderInfo.offsets.push_back(p);
				staticWorldRenderInfo.offsetsShadow.push_back(p);
				if (this->blocks[x][y].isBlock()) {
					staticWorldRenderInfo.textureIDs.push_back(this->blocks[x][y].getID());
				}
				else {
					staticWorldRenderInfo.textureIDs.push_back(1);
				}
			}
		}
	}
}

bool Platform::canMoveLocal(GameState& gameState, Activity::DIR dir, ActivityIgnoringGroup& ignore) {
	glm::ivec2 movedOrigin = origin + getDirection(dir);
	glm::ivec2 p1 = floordiv(movedOrigin, CHUNKSIZE);
	glm::ivec2 p2 = floordiv(movedOrigin + this->size, CHUNKSIZE);

	glm::ivec2 localSize = 1 + p2 - p1;
	std::vector<std::vector<StaticWorldChunk*>> nearChunks(localSize.x, std::vector<StaticWorldChunk*>(localSize.y, nullptr));

	// TODO: move logic of this loop to staticworld?
	for (int32_t i = p1.x; i <= p2.x; i++) {
		for (int32_t j = p1.y; j <= p2.y; j++) {
			nearChunks[i - p1.x][j - p1.y] = gameState.staticWorld.getChunkByIndex(i, j);
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

void Platform::removeMoveableTracesLocal(GameState& gameState) {
	for (auto& dir : this->blockedDirections[Activity::FLIP(movementDirection)]) {
		gameState.staticWorld.removeTraceFilter(origin + dir, selfHandle);
	}
}

void Platform::leaveMoveableTracesLocal(GameState& gameState) {
	for (auto& dir : this->blockedDirections[movementDirection]) {
		gameState.staticWorld.leaveTrace(origin + dir, selfHandle);
	}
}

bool Platform::canActivityLocal(GameState& gameState, int32_t useType) {
	return false;
}

void Platform::removeActivityTracesLocal(GameState& gameState) {
}

void Platform::leaveActivityTracesLocal(GameState& gameState) {
}

void Platform::save(Saver& saver) {
	Activity::save(saver);
	saver.store<glm::ivec2>(this->size);
}

bool Platform::load(Loader& loader) {
	Activity::load(loader);
	loader.retrieve<glm::ivec2>(this->size);

	this->blocks = std::vector<std::vector<Block>>(this->size[0], std::vector<Block>(this->size[1], Block(0)));
	int32_t textureID = Locator<BlockIDTextures>::get()->getBlockTextureID("mossy_cobblestone.dds");
	for (int32_t i = 0; i < this->size[0]; i++) {
		for (int32_t j = 0; j < this->size[1]; j++) {
			auto p = origin + glm::ivec2(i, j);
			this->blocks[i][j].setID(textureID);
		}
	}

	calculateBlockedDirections();
	return true;
}

void Platform::getTreeMembers(std::vector<Activity*>& members) {
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
	for (int32_t i = 0; i < this->size.x; i++) {
		for (int32_t j = 0; j < this->size.y; j++) {
			auto p = origin + glm::ivec2(i, j);
			gameState.staticWorld.leaveTrace(p, selfHandle);
		}
	}
}

void Platform::removeTracesLocalForced(GameState& gameState) {
	for (int32_t i = 0; i < this->size.x; i++) {
		for (int32_t j = 0; j < this->size.y; j++) {
			auto p = origin + glm::ivec2(i, j);
			gameState.staticWorld.removeTraceForced(p);
		}
	}
}

void Platform::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	Activity::applyActivityLocalForced(gameState, type, pace);
}
