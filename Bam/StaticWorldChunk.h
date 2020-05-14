#pragma once

#include "Block.h"
#include "common.h"

class PerlinNoise;
struct RenderInfo;
class Saver;
class Loader;

class StaticWorldChunk
{
public:
	glm::ivec2 getPosition();

	void setBlock(glm::ivec2 pos, int32_t blockID, bool occlude);
	void appendStaticRenderInfo(RenderInfo& renderInfo);

	bool isOccupied(glm::ivec2& pos);
	bool isOccupied(glm::ivec2& pos, ActivityIgnoringGroup& ignore);

	bool load(Loader& loader);
	bool save(Saver& saver);

	StaticWorldChunk(glm::ivec2 pos);

	StaticWorldChunk(glm::ivec2 _position, bool empty);
	StaticWorldChunk() = default;

private:
	friend class StaticWorld;
	glm::ivec2 position;
	std::array<std::array<Block, CHUNKSIZE>, CHUNKSIZE> staticWorld;

private:
	void fill(PerlinNoise& noise);
	void calculateOcclusions();
};

