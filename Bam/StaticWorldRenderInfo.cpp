#include "common.h"
#include "StaticWorldRenderInfo.h"

void StaticWorldRenderInfo::addBlockWithShadow(glm::vec2 pos, int32_t texture) {
	this->addBlockWithShadow(pos, texture, 0);
}

void StaticWorldRenderInfo::addBlockWithShadow(glm::vec2 pos, int32_t texture, int32_t rotation) {
	this->offsets.push_back(pos);
	this->offsetsShadow.push_back(pos);
	this->textureIDs.push_back(texture);
	this->rotations.push_back(rotation);
}

void StaticWorldRenderInfo::addBlockWithoutShadow(glm::vec2 pos, int32_t texture) {
	this->addBlockWithoutShadow(pos, texture, 0);
}

void StaticWorldRenderInfo::addBlockWithoutShadow(glm::vec2 pos, int32_t texture, int32_t rotation) {
	this->offsets.push_back(pos);
	this->textureIDs.push_back(texture);
	this->rotations.push_back(rotation);
}
