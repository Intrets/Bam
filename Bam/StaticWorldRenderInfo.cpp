#include "common.h"

#include "StaticWorldRenderInfo.h"
#include "Block.h"

void StaticWorldRenderInfo::addBlock(glm::vec2 pos, ShapedBlock const& shapedBlock) {
	this->blockRenderInfos.push_back({
		pos,
		shapedBlock.getRotation(),
		shapedBlock.getTexture(),
		shapedBlock.getStencil() }
	);
}

void StaticWorldRenderInfo::addBlockBack(glm::vec2 pos, ShapedBlock const& shapedBlock) {
	this->blockRenderInfosBack.push_back({
		pos,
		shapedBlock.getRotation(),
		shapedBlock.getTexture(),
		shapedBlock.getStencil() }
	);
}

void StaticWorldRenderInfo::addBlockFront(glm::vec2 pos, ShapedBlock const& shapedBlock) {
	this->blockRenderInfosFront.push_back({
		pos,
		shapedBlock.getRotation(),
		shapedBlock.getTexture(),
		shapedBlock.getStencil() }
	);
}
