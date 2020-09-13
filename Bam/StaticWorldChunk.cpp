#include "StaticWorldChunk.h"

#include "common.h"

#include "PerlinNoise.h"
#include "RenderInfo.h"

#include "Loader.h"
#include "Saver.h"
#include <iostream>

glm::ivec2 StaticWorldChunk::getPosition() {
	return this->position;
}

void StaticWorldChunk::setBlock(glm::ivec2 pos, int32_t blockID, bool occlude) {
	this->staticWorld[pos.x][pos.y].setBlockID(blockID);
}

void StaticWorldChunk::appendStaticRenderInfo(RenderInfo& renderInfo) {
	for (int32_t i = 0; i < CHUNKSIZE; i++) {
		for (int32_t j = 0; j < CHUNKSIZE; j++) {
			if (this->staticWorld[i][j].isNonAirBlock()) {
				renderInfo.staticWorldRenderInfo.addBlockWithShadow(glm::vec2(i, j) + glm::vec2(this->position), this->staticWorld[i][j].getTexture());

			}
			if (this->staticWorld[i][j].isActivity()) {
				renderInfo.debugRenderInfo.addPoint(glm::vec2(i, j) + glm::vec2(this->position) + glm::vec2(0.5f));
			}
		}
	}
}

bool StaticWorldChunk::isOccupied(glm::ivec2& pos) {
	return this->staticWorld[pos.x][pos.y].isOccupied();
}

bool StaticWorldChunk::isOccupied(glm::ivec2& pos, ActivityIgnoringGroup const& ignore) {
	return this->staticWorld[pos.x][pos.y].isOccupied(ignore);
}

bool StaticWorldChunk::load(Loader& loader) {
	loader.retrieve(this->position);
	for (int32_t i = 0; i < CHUNKSIZE; i++) {
		for (int32_t j = 0; j < CHUNKSIZE; j++) {
			this->staticWorld[i][j].load(loader);
		}
	}
	return true;
}

bool StaticWorldChunk::save(Saver& saver) {
	saver.store(this->position);
	for (int32_t i = 0; i < CHUNKSIZE; i++) {
		for (int32_t j = 0; j < CHUNKSIZE; j++) {
			this->staticWorld[i][j].save(saver);
		}
	}
	return true;
}

StaticWorldChunk::StaticWorldChunk(glm::ivec2 pos) : StaticWorldChunk(pos, false) {
}

void StaticWorldChunk::fill(PerlinNoise& noise) {
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int32_t> uni(4, 11);

	if (this->position.y > -1) {
		return;
	}
	for (int32_t i = 0; i < CHUNKSIZE; i++) {
		for (int32_t j = 0; j < CHUNKSIZE; j++) {
			if (!noise.block[i][j]) {
				if (noise.values[i][j] < 0.75f) {
					setBlock(glm::ivec2(i, j), 1, false);
				}
				else {
					setBlock(glm::ivec2(i, j), 2, false);
				}
			}
		}
	}
}

StaticWorldChunk::StaticWorldChunk(glm::ivec2 _position, bool empty) : position(_position) {
	if (!empty) {
		PerlinNoise perlinNoise(this->position, glm::ivec2(CHUNKSIZE));
		perlinNoise.fillTexture(false);
		fill(perlinNoise);
	}
}
