#include "common.h"
#include "Platform.h"
#include "BlockIDTextures.h"
#include "GameState.h"
#include "StaticWorldRenderer.h"
#include "Anchor.h"
#include "StaticWorldChunk.h"	
#include "Modifyables.h"

//#include "Saver.h"
//#include "Loader.h"
#include "RenderInfo.h"

void Platform::calculateBlockedDirections() {
	for (int i = 0; i < 4; i++) {
		blockedDirections[i].clear();
	}
	for (int x = 0; x < size.x; x++) {
		{
			int y = 0;
			if (blocks[x][y].isOccupied()) {
				blockedDirections[MOVEABLE::DOWN].push_back(glm::ivec2(x, y - 1));
			}
		}
		{
			int y = size.y - 1;
			if (blocks[x][y].isOccupied()) {
				blockedDirections[MOVEABLE::UP].push_back(glm::ivec2(x, y + 1));
			}
		}
		for (int y = 0; y < size.y - 1; y++) {
			if (blocks[x][y].isOccupied() && !blocks[x][y + 1].isOccupied()) {
				blockedDirections[MOVEABLE::UP].push_back(glm::ivec2(x, y + 1));
			}
			else if (!blocks[x][y].isOccupied() && blocks[x][y + 1].isOccupied()) {
				blockedDirections[MOVEABLE::DOWN].push_back(glm::ivec2(x, y));
			}
		}
	}
	for (int y = 0; y < size.y; y++) {
		{
			int x = 0;
			if (blocks[x][y].isOccupied()) {
				blockedDirections[MOVEABLE::LEFT].push_back(glm::ivec2(x - 1, y));
			}
		}
		{
			int x = size.x - 1;
			if (blocks[x][y].isOccupied()) {
				blockedDirections[MOVEABLE::RIGHT].push_back(glm::ivec2(x + 1, y));
			}
		}
		for (int x = 0; x < size.x - 1; x++) {
			if (blocks[x][y].isOccupied() && !blocks[x + 1][y].isOccupied()) {
				blockedDirections[MOVEABLE::RIGHT].push_back(glm::ivec2(x + 1, y));
			}
			else if (!blocks[x][y].isOccupied() && blocks[x + 1][y].isOccupied()) {
				blockedDirections[MOVEABLE::LEFT].push_back(glm::ivec2(x, y));
			}
		}
	}
}

Platform::Platform(Handle self, GameState& gameState, glm::ivec2 _size, glm::ivec2 pos, bool leaveTraces) :
	Activity(self, pos),
	size(_size),
	blocks(size[0], std::vector<Block>(size[1], Block(0))) {
	int textureID = Locator<BlockIDTextures>::getService()->getBlockTextureID("mossy_cobblestone.dds");
	for (int i = 0; i < size[0]; i++) {
		for (int j = 0; j < size[1]; j++) {
			auto p = origin + glm::ivec2(i, j);
			blocks[i][j].setID(textureID);
		}
	}
	blocks[0][1].setID(4);
	blocks[1][0].setID(5);
	if (leaveTraces) {
		fillTracesLocalForced(gameState);
	}
	calculateBlockedDirections();
}

void Platform::rotateForcedLocal(glm::ivec2 center, MOVEABLE::ROT rotation) {
	auto d = origin - center;
	size = glm::ivec2(size.y, size.x);
	auto old = blocks;
	blocks.clear();
	blocks.resize(size.x, std::vector<Block>(size.y));
	switch (rotation) {
		case MOVEABLE::ROT::CLOCKWISE:
			d = glm::ivec2(d.y, -d.x - size.y);
			for (int i = 0; i < size.x; i++) {
				for (int j = 0; j < size.y; j++) {
					blocks[i][j] = old[size.y - 1 - j][i];
				}
			}
			calculateBlockedDirections();
			break;
		case MOVEABLE::ROT::COUNTERCLOCKWISE:
			d = glm::ivec2(-d.y - size.x, d.x);
			for (int i = 0; i < size.x; i++) {
				for (int j = 0; j < size.y; j++) {
					blocks[i][j] = old[j][size.x - 1 - i];
				}
			}
			calculateBlockedDirections();
			break;
		default:
			break;
	}
	origin = center + d;
}

void Platform::appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo) {
	int tick = gameState.tick;
	glm::vec2 v = glm::vec2(origin);
	if (moving) {
		float scale = static_cast<float>(tick - movingTickStart) / movingPace;
		v += scale * glm::vec2(getDirection(movementDirection));
	}
	//renderInfo.selectionRenderInfo.addBox(v, v + glm::vec2(size) - glm::vec2(1.0, 1.0));
	renderInfo.selectionRenderInfo.addBox(v, v + glm::vec2(size));
	renderInfo.debugRenderInfo.addPoint(v);

}

void Platform::appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	int tick = gameState.tick;
	glm::vec2 v = glm::vec2(origin);
	if (moving) {
		float scale = static_cast<float>(tick - movingTickStart) / movingPace;
		v += scale * glm::vec2(getDirection(movementDirection));
	}
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			// TODO: remove debugging textures 
			if (blocks[x][y].isOccupied()) {
				glm::vec2 p(x, y);
				p += v;
				staticWorldRenderInfo.offsets.push_back(p);
				staticWorldRenderInfo.offsetsShadow.push_back(p);
				if (blocks[x][y].isBlock()) {
					staticWorldRenderInfo.textureIDs.push_back(blocks[x][y].getID());
				}
				else {
					staticWorldRenderInfo.textureIDs.push_back(1);
				}
			}
		}
	}
}

bool Platform::canMoveLocal(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) {
	glm::ivec2 movedOrigin = origin + getDirection(dir);
	glm::ivec2 p1 = floordiv(movedOrigin, CHUNKSIZE);
	glm::ivec2 p2 = floordiv(movedOrigin + size, CHUNKSIZE);

	glm::ivec2 localSize = 1 + p2 - p1;
	std::vector<std::vector<StaticWorldChunk*>> nearChunks(localSize.x, std::vector<StaticWorldChunk*>(localSize.y, nullptr));

	// TODO: move logic of this loop to staticworld?
	for (int i = p1.x; i <= p2.x; i++) {
		for (int j = p1.y; j <= p2.y; j++) {
			nearChunks[i - p1.x][j - p1.y] = gameState.staticWorld.getChunkByIndex(i, j);
		}
	}
	bool blocked = false;
	for (glm::ivec2& v_ : blockedDirections[dir]) {
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
	for (auto& dir : blockedDirections[(movementDirection + 2) % 4]) {
		gameState.staticWorld.removeTraceFilter(origin + dir, selfHandle);
	}
}

void Platform::leaveMoveableTracesLocal(GameState& gameState) {
	for (auto& dir : blockedDirections[movementDirection]) {
		gameState.staticWorld.leaveTrace(origin + dir, selfHandle);
	}
}

bool Platform::canActivityLocal(GameState& gameState, int useType) {
	return false;
}

void Platform::removeActivityTracesLocal(GameState& gameState) {
}

void Platform::leaveActivityTracesLocal(GameState& gameState) {
}

void Platform::save(Saver& saver) {
	Activity::save(saver);
	saver.store<glm::ivec2>(size);
}

bool Platform::load(Loader& loader) {
	Activity::load(loader);
	loader.retrieve<glm::ivec2>(size);

	blocks = std::vector<std::vector<Block>>(size[0], std::vector<Block>(size[1], Block(0)));
	int textureID = Locator<BlockIDTextures>::getService()->getBlockTextureID("mossy_cobblestone.dds");
	for (int i = 0; i < size[0]; i++) {
		for (int j = 0; j < size[1]; j++) {
			auto p = origin + glm::ivec2(i, j);
			blocks[i][j].setID(textureID);
		}
	}

	calculateBlockedDirections();
	return true;
}

void Platform::fillModifyingMap(ModifyerBase& modifyer) {
	Activity::fillModifyingMap(modifyer);
}

void Platform::modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) {
	auto& modifyer = Locator<Modifyer<Platform>>::getService()->modifyables;
	if (modifyer.count(name) != 0) {
		modifyer[name]->modify(this, gameState, value);
	}
}

std::stringstream& Platform::getMembers(std::stringstream& out) {
	out << "^ Platform members: ^\n";
	for (auto& member : Locator<Modifyer<Platform>>::getService()->modifyables) {
		out << member.first << ": ";
		member.second->toStream(this, out) << "\n";
	}
	out << "v Platform members: v\n";
	return out;
}

void Platform::getTreeMembers(std::vector<Activity*>& members) {
	members.push_back(this);
}

bool Platform::canFillTracesLocal(GameState& gameState) {
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++) {
			auto p = origin + glm::ivec2(i, j);
			if (gameState.staticWorld.isOccupied(p)) {
				return false;
			}
		}
	}
	return true;
}

void Platform::fillTracesLocalForced(GameState& gameState) {
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++) {
			auto p = origin + glm::ivec2(i, j);
			gameState.staticWorld.leaveTrace(p, selfHandle);
		}
	}
}

void Platform::removeTracesLocalForced(GameState& gameState) {
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++) {
			auto p = origin + glm::ivec2(i, j);
			gameState.staticWorld.removeTraceForced(p);
		}
	}
}

void Platform::applyActivityLocalForced(GameState& gameState, int type, int pace) {
	Activity::applyActivityLocalForced(gameState, type, pace);
}
