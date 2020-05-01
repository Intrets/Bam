#include "StaticWorldChunk.h"

#include "common.h"

#include "PerlinNoise.h"
#include "RenderInfo.h"

glm::ivec2 StaticWorldChunk::getPosition() {
	return position;
}

void StaticWorldChunk::setBlock(glm::ivec2 pos, int blockID, bool occlude) {
	staticWorld[pos.x][pos.y].blockID = blockID;
	static std::array < glm::ivec2, 4> dirs;
	if (occlude) {
		for (auto dir : dirs) {
			glm::ivec2 t = pos + dir;
			if (t.x >= 0 && t.y >= 0 && t.x < CHUNKSIZE && t.y < CHUNKSIZE) {
				if (staticWorld[t.x][t.y].isBlock()) {
					staticWorld[t.x][t.y].occluded = true;
				}
			}
		}
	}
}

void StaticWorldChunk::appendStaticRenderInfo(RenderInfo& renderInfo) {
	for (int i = 0; i < CHUNKSIZE; i++) {
		for (int j = 0; j < CHUNKSIZE; j++) {
			if (staticWorld[i][j].isBlock()) {
				renderInfo.staticWorldRenderInfo.offsets.push_back(glm::vec2(i, j) + glm::vec2(position));
				//renderInfo.staticWorldRenderInfo.textureIDs.push_back(staticWorldChunk->staticWorld[i][j].blockID);
				renderInfo.staticWorldRenderInfo.textureIDs.push_back(staticWorld[i][j].blockID);

				if (!staticWorld[i][j].isOccluded()) {
					renderInfo.staticWorldRenderInfo.offsetsShadow.push_back(glm::vec2(i, j) + glm::vec2(position));
				}
			}
		}
	}
}

StaticWorldChunk::StaticWorldChunk(glm::ivec2 pos) : StaticWorldChunk(pos, false) {
}

void StaticWorldChunk::fill(PerlinNoise& noise) {
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(4, 11);

	if (position.y > -1) {
		return;
	}
	for (int i = 0; i < CHUNKSIZE; i++) {
		for (int j = 0; j < CHUNKSIZE; j++) {
			if (!noise.block[i][j]) {
				if (noise.values[i][j] < 0.75f) {
					setBlock(glm::ivec2(i, j), uni(rng), false);
				}
				else {
					setBlock(glm::ivec2(i, j), 3, false);
				}
			}
		}
	}
	calculateOcclusions();
}

void StaticWorldChunk::calculateOcclusions() {
	for (int i = 1; i < CHUNKSIZE - 1; i++) {
		for (int j = 1; j < CHUNKSIZE - 1; j++) {
			if (staticWorld[i][j].isOccupied()) {
				if (staticWorld[i + 1][j].isOccupied() &&
					staticWorld[i][j + 1].isOccupied() &&
					staticWorld[i - 1][j].isOccupied() &&
					staticWorld[i][j - 1].isOccupied()) {
					staticWorld[i][j].occluded = true;
				}
			}
		}
	}
}

StaticWorldChunk::StaticWorldChunk(glm::ivec2 _position, bool empty) : position(_position) {
	if (!empty) {
		PerlinNoise perlinNoise(_position, glm::ivec2(CHUNKSIZE));
		perlinNoise.fillTexture(false);
		fill(perlinNoise);
	}
}
