#include "common.h"
#include "StaticWorldRenderInfo.h"

void StaticWorldRenderInfo::addBlockWithShadow(glm::vec2 pos, int32_t blockID) {
	offsets.push_back(pos);
	offsetsShadow.push_back(pos);
	textureIDs.push_back(blockID);
}

void StaticWorldRenderInfo::addBlockWithoutShadow(glm::vec2 pos, int32_t blockID) {
	offsets.push_back(pos);
	textureIDs.push_back(blockID);
}
