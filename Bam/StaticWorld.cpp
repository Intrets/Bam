#include "common.h"

#include "StaticWorld.h"

#include "RenderInfo.h"
#include "StaticWorldChunk.h"
#include "Saver.h"
#include "Loader.h"
#include "Activity.h"
#include "ReferenceManager.h"
#include "Block.h"
#include "WorldBlock.h"

std::optional<WeakReference<Activity, Activity>> StaticWorld::getActivity(glm::ivec2 pos) {
	auto block = this->getBlockRef(pos);
	return block->getActivityMaybe();
}

std::optional<WeakReference<Activity, Activity>> StaticWorld::getActivity(glm::vec2 pos) {
	return this->getActivity(glm::ivec2(glm::floor(pos)));
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
	getBlockRef(pos)->setTrace(m);
}

void StaticWorld::removeTraceForced(glm::ivec2 pos) {
	getBlockRef(pos)->removeTrace();
}

void StaticWorld::removeTraceFilter(glm::ivec2 pos, Handle m) {	
	getBlockRef(pos)->removeTrace(m);
}

bool StaticWorld::load(Loader& loader) {
	this->world.clear();
	size_t size;
	loader.retrieve(size);
	for (int32_t i = 0; i < size; i++) {
		glm::ivec2 key;
		loader.retrieve(key);
		auto chunk = std::make_unique<StaticWorldChunk>(key * CHUNKSIZE, true);
		chunk->load(loader);
		this->world[key] = std::move(chunk);
	}
	return true;
}

bool StaticWorld::save(Saver& saver) {
	saver.store(this->world.size());
	for (auto& chunk : this->world) {
		saver.store(chunk.first);
		chunk.second->save(saver);
	}
	return true;
}

StaticWorldChunk* StaticWorld::getChunkByIndex(int32_t i, int32_t j) {
	auto search = this->world.find(glm::ivec2(i, j));
	StaticWorldChunk* res;
	if (search == this->world.end()) {
		auto newChunk = std::make_unique<StaticWorldChunk>(glm::vec2(i * CHUNKSIZE, j * CHUNKSIZE));
		res = newChunk.get();
		this->world.insert(
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

WorldBlock* StaticWorld::getBlockRef(glm::ivec2 pos) {
	auto r = floordivmod(pos, CHUNKSIZE);
	auto const& global = r.first;
	auto const& local = r.second;
	return &(getChunkByIndex(global.x, global.y)->staticWorld[local.x][local.y]);
}

WorldBlock* StaticWorld::getBlockRef(glm::vec2 pos) {
	return this->getBlockRef(glm::ivec2(glm::floor(pos)));
}

void StaticWorld::setBlock(ShapedBlock block, glm::ivec2 pos) {
	auto [global, local] = floordivmod(pos, CHUNKSIZE);
	getChunkByIndex(global.x, global.y)->staticWorld[local.x][local.y].setBlock(block);
}

void StaticWorld::setBlock(ShapedBlock block, glm::vec2 pos) {
	this->setBlock(std::move(block), glm::ivec2(glm::floor(pos)));
}

bool StaticWorld::isOccupied(glm::ivec2 pos, ActivityIgnoringGroup const& ignore) {
	auto p = floordivmod(pos, CHUNKSIZE);
	return getChunkByIndex(p.first.x, p.first.y)->isOccupied(p.second, ignore);
}

bool StaticWorld::isOccupied(glm::ivec2 pos) {
	auto p = floordivmod(pos, CHUNKSIZE);
	return getChunkByIndex(p.first.x, p.first.y)->isOccupied(p.second);
}
