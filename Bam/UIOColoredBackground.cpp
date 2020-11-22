#include "common.h"

#include "UIOColoredBackground.h"
#include "RenderInfo.h"

UIOColoredBackground::UIOColoredBackground(Handle self) {
	this->selfHandle = self;
}

int32_t UIOColoredBackground::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	depth = this->UIOBaseSingle::addRenderInfo(gameState, renderInfo, depth);
	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.getBottomLeft(), this->screenRectangle.getTopRight(), this->color, depth++);
	return depth;
}
