#pragma once

#include "common.h"

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
	std::vector<glm::vec2> offsets;
	std::vector<int32_t> rotations;
	std::vector<glm::vec2> offsetsShadow;

	std::vector<int32_t> textureIDs;

	std::vector<BlockRenderInfo> blockRenderInfosFront;
	std::vector<BlockRenderInfo> blockRenderInfos;
	std::vector<BlockRenderInfo> blockRenderInfosBack;

	void addBlockWithShadow(glm::vec2 pos, int32_t texture);
	void addBlockWithShadow(glm::vec2 pos, int32_t texture, int32_t rotation);
	void addBlockWithoutShadow(glm::vec2 pos, int32_t texture);
	void addBlockWithoutShadow(glm::vec2 pos, int32_t texture, int32_t rotation);

	void addBlock(glm::vec2 pos, ShapedBlock const& shapedBlock);
	void addBlockBack(glm::vec2 pos, ShapedBlock const& shapedBlock);
	void addBlockFront(glm::vec2 pos, ShapedBlock const& shapedBlock);
};
