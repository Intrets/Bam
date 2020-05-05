#include "StaticWorld.h"

#include "common.h"

#include "RenderInfo.h"
#include "StaticWorldChunk.h"

void StaticWorld::appendStaticRenderInfo(RenderInfo& renderInfo) {
	auto& cameraInfo = renderInfo.cameraInfo;
	auto p0 = cameraInfo.camPos - glm::vec2(cameraInfo.viewPort);
	auto p1 = cameraInfo.camPos + glm::vec2(cameraInfo.viewPort);
	float size = CHUNKSIZE;
	auto i0 = glm::ivec2(glm::floor(p0 / size));
	auto i1 = glm::ivec2(glm::floor(p1 / size));

	for (int ci = i0.x; ci <= i1.x; ci++) {
		for (int cj = i0.y; cj <= i1.y; cj++) {
			auto staticWorldChunk = getChunkByIndex(ci, cj);
			staticWorldChunk->appendStaticRenderInfo(renderInfo);
			if (renderInfo.staticWorldRenderInfo.offsets.size() > MAX_STATIC_DRAW) {
				return;
			}
		}
	}
}

void StaticWorld::leaveTrace(glm::ivec2 pos, Handle m) {
	auto r = floordivmod(pos, CHUNKSIZE);
	auto global = r.first;
	auto local = r.second;
	getChunkByIndex(global.x, global.y)->staticWorld[local.x][local.y].setID(1);
	getChunkByIndex(global.x, global.y)->staticWorld[local.x][local.y].setM(m);
}

void StaticWorld::removeTrace(glm::ivec2 pos) {
	auto r = floordivmod(pos, CHUNKSIZE);
	auto global = r.first;
	auto local = r.second;
	getChunkByIndex(global.x, global.y)->staticWorld[local.x][local.y].setID(0);
	getChunkByIndex(global.x, global.y)->staticWorld[local.x][local.y].setM(0);
}

void StaticWorld::removeTrace(glm::ivec2 pos, Handle m) {
	auto r = floordivmod(pos, CHUNKSIZE);
	auto global = r.first;
	auto local = r.second;
	if (getChunkByIndex(global.x, global.y)->staticWorld[local.x][local.y].m.handle == m) {
		getChunkByIndex(global.x, global.y)->staticWorld[local.x][local.y].blockID = 0;
		getChunkByIndex(global.x, global.y)->staticWorld[local.x][local.y].m.handle = 0;
	}
}

StaticWorldChunk* StaticWorld::getChunkByIndex(int i, int j) {
	auto search = world.find(glm::ivec2(i, j));
	StaticWorldChunk* res;
	if (search == world.end()) {
		auto newChunk = std::make_unique<StaticWorldChunk>(glm::vec2(i * CHUNKSIZE, j * CHUNKSIZE));
		res = newChunk.get();
		world.insert(
			std::make_pair<glm::ivec2, std::unique_ptr<StaticWorldChunk>>(
				glm::ivec2(i, j),
				std::move(newChunk)
				)
		);
	}
	else {
		res = search->second.get();
	}
	return res;
}

StaticWorldChunk* StaticWorld::getChunkByCoords(glm::vec2 pos) {
	float size = CHUNKSIZE;
	glm::ivec2 t = glm::floor(pos / size);
	return getChunkByIndex(t.x, t.y);
}
