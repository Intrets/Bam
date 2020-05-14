#pragma once

#include "common.h"

struct StaticWorldRenderInfo
{
	std::vector<glm::vec2> offsets;
	std::vector<glm::vec2> offsetsShadow;
	std::vector<int32_t> textureIDs;

	void addBlockWithShadow(glm::vec2 pos, int32_t blockID);
	void addBlockWithoutShadow(glm::vec2 pos, int32_t blockID);
};
