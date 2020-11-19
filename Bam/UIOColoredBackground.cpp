#include "common.h"

#include "UIOColoredBackground.h"
#include "RenderInfo.h"

UIOColoredBackground::UIOColoredBackground(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	this->addElement(std::move(main_));
	this->selfHandle = self;
}

UIOColoredBackground::UIOColoredBackground(Handle self, glm::vec4 c, UniqueReference<UIOBase, UIOBase> main_) {
	this->addElement(std::move(main_));
	this->selfHandle = self;
	this->color = c;
}

int32_t UIOColoredBackground::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	depth = this->UIOBaseSingle::addRenderInfo(gameState, renderInfo, depth);
	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.getBottomLeft(), this->screenRectangle.getTopRight(), this->color, depth++);
	return depth;
}
