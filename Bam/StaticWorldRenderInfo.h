#pragma once

#include "common.h"

struct StaticWorldRenderInfo
{
	std::vector<glm::vec2> offsets;
	std::vector<glm::vec2> offsetsShadow;
	std::vector<int> textureIDs;

	void addBlockWithShadow(glm::vec2 pos, int blockID);
	void addBlockWithoutShadow(glm::vec2 pos, int blockID);
};
