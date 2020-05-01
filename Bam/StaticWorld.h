#pragma once

#include "common.h"

#include <unordered_map>

class StaticWorldChunk;
struct RenderInfo;

class StaticWorld
{
public:
	void appendStaticRenderInfo(RenderInfo& renderInfo);

private:
	struct hash
	{
		std::size_t operator()(glm::ivec2 const& s) const noexcept {
			return s.x + (1 << 16) * s.y;
		}
	};

	std::unordered_map<glm::ivec2, std::unique_ptr<StaticWorldChunk>, hash> world;

	StaticWorldChunk* getChunkByIndex(int i, int j);
	StaticWorldChunk* getChunkByCoords(glm::vec2 pos);
};

