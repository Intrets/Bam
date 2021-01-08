#pragma once

#include <GLM/glm.hpp>

class ShapedBlock;

struct BlockRenderInfo
{
	glm::vec2 pos;
	int32_t rotation;
	int32_t texture;
	int32_t stencil;
};

struct StaticWorldRenderInfo
{
	std::vector<BlockRenderInfo> blockRenderInfosFront;
	std::vector<BlockRenderInfo> blockRenderInfos;
	std::vector<BlockRenderInfo> blockRenderInfosBack;

	void addBlock(glm::vec2 pos, ShapedBlock const& shapedBlock);
	void addBlockBack(glm::vec2 pos, ShapedBlock const& shapedBlock);
	void addBlockFront(glm::vec2 pos, ShapedBlock const& shapedBlock);
};
