#include "StaticWorld.h"

#include "common.h"

#include "RenderInfo.h"
#include "StaticWorldChunk.h"
#include "Saver.h"
#include "Loader.h"
#include "Block.h"

std::optional<WeakReference<Activity, Activity>> StaticWorld::getActivity(glm::ivec2 pos) {
	auto block = getBlockRef(pos);
	if (block->isActivity()) {
		return std::make_optional(block->m);
	}
	else {
		return std::nullopt;
	}
}

void StaticWorld::appendStaticRenderInfo(RenderInfo& renderInfo) {
	auto& cameraInfo = renderInfo.cameraInfo;
	auto p0 = cameraInfo.camPos - glm::vec2(cameraInfo.viewPort);
	auto p1 = cameraInfo.camPos + glm::vec2(cameraInfo.viewPort);
	float size = CHUNKSIZE;
	auto i0 = glm::ivec2(glm::floor(p0 / size));
	auto i1 = glm::ivec2(glm::floor(p1 / size));

	for (int32_t ci = i0.x; ci <= i1.x; ci++) {
		for (int32_t cj = i0.y; cj <= i1.y; cj++) {
			auto staticWorldChunk = getChunkByIndex(ci, cj);
			staticWorldChunk->appendStaticRenderInfo(renderInfo);
			if (renderInfo.staticWorldRenderInfo.offsets.size() > MAX_STATIC_DRAW) {
				return;
			}
		}
	}
}

void StaticWorld::leaveTrace(glm::ivec2 pos, Handle m) {
	getBlockRef(pos)->m.handle = m;
}

void StaticWorld::removeTraceForced(glm::ivec2 pos) {
	getBlockRef(pos)->m.handle = 0;
}

void StaticWorld::removeTraceFilter(glm::ivec2 pos, Handle m) {	
	auto block = getBlockRef(pos);
	if (block->m.handle == m) {
		block->m.handle = 0;
	}
}

bool StaticWorld::load(Loader& loader) {
	world.clear();
	int32_t size;
	loader.retrieve(size);
	for (int32_t i = 0; i < size; i++) {
		glm::ivec2 key;
		loader.retrieve(key);
		auto chunk = std::make_unique<StaticWorldChunk>(key * CHUNKSIZE, true);
		chunk->load(loader);
		world[key] = std::move(chunk);
	}
	return true;
}

bool StaticWorld::save(Saver& saver) {
	saver.store(world.size());
	for (auto& chunk : world) {
		saver.store(chunk.first);
		chunk.second->save(saver);
	}
	return true;
}

StaticWorldChunk* StaticWorld::getChunkByIndex(int32_t i, int32_t j) {
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
	glm::ivec2 t = floordiv(pos, CHUNKSIZE);
	return getChunkByIndex(t.x, t.y);
}

Block* StaticWorld::getBlockRef(glm::ivec2 pos) {
	auto r = floordivmod(pos, CHUNKSIZE);
	auto global = r.first;
	auto local = r.second;
	return &(getChunkByIndex(global.x, global.y)->staticWorld[local.x][local.y]);
}

bool StaticWorld::isOccupied(glm::ivec2 pos, ActivityIgnoringGroup& ignore) {
	auto p = floordivmod(pos, CHUNKSIZE);
	return getChunkByIndex(p.first.x, p.first.y)->isOccupied(p.second, ignore);
}

bool StaticWorld::isOccupied(glm::ivec2 pos) {
	auto p = floordivmod(pos, CHUNKSIZE);
	return getChunkByIndex(p.first.x, p.first.y)->isOccupied(p.second);
}
