#pragma once

#include "common.h"

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

	std::vector<BlockRenderInfo> blockRenderInfos;

	void addBlockWithShadow(glm::vec2 pos, int32_t texture);
	void addBlockWithShadow(glm::vec2 pos, int32_t texture, int32_t rotation);
	void addBlockWithoutShadow(glm::vec2 pos, int32_t texture);
	void addBlockWithoutShadow(glm::vec2 pos, int32_t texture, int32_t rotation);
};
