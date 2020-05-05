#pragma once

#include "Block.h"
#include "common.h"

class PerlinNoise;
struct RenderInfo;

class StaticWorldChunk
{
public:
	glm::ivec2 getPosition();

	void setBlock(glm::ivec2 pos, int blockID, bool occlude);
	void appendStaticRenderInfo(RenderInfo& renderInfo);

	bool isOccupied(glm::ivec2& pos);
	bool isOccupied(glm::ivec2& pos, ActivityIgnoringGroup& ignore);

	StaticWorldChunk(glm::ivec2 pos);

private:
	friend class StaticWorld;
	glm::ivec2 position;
	std::array<std::array<Block, CHUNKSIZE>, CHUNKSIZE> staticWorld;

private:
	void fill(PerlinNoise& noise);
	void calculateOcclusions();

	StaticWorldChunk(glm::ivec2 _position, bool empty);
	StaticWorldChunk() = default;
};

