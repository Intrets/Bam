#include "common.h"
#include "StaticWorldRenderInfo.h"

void StaticWorldRenderInfo::addBlockWithShadow(glm::vec2 pos, int32_t texture) {
	this->offsets.push_back(pos);
	this->offsetsShadow.push_back(pos);
	this->textureIDs.push_back(texture);
}

void StaticWorldRenderInfo::addBlockWithoutShadow(glm::vec2 pos, int32_t texture) {
	this->offsets.push_back(pos);
	this->textureIDs.push_back(texture);
}
