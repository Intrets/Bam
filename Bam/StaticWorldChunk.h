#pragma once

#include "common.h"

#include "WorldBlock.h"
#include "Enums.h"

class PerlinNoise;
struct RenderInfo;
class Saver;
class Loader;
class ActivityIgnoringGroup;

class StaticWorldChunk
{
public:
	glm::ivec2 getPosition();

	void setBlock(glm::ivec2 pos, int32_t blockID, ACTIVITY::DIR rotation);
	void appendStaticRenderInfo(RenderInfo& renderInfo);

	bool isOccupied(glm::ivec2& pos);
	bool isOccupied(glm::ivec2& pos, ActivityIgnoringGroup const& ignore);

	bool load(Loader& loader);
	bool save(Saver& saver);

	StaticWorldChunk(glm::ivec2 pos);

	StaticWorldChunk(glm::ivec2 _position, bool empty);
	StaticWorldChunk() = default;

private:
	friend class StaticWorld;
	glm::ivec2 position;
	std::array<std::array<WorldBlock, CHUNKSIZE>, CHUNKSIZE> staticWorld;

private:
	void fill(PerlinNoise& noise);
};

