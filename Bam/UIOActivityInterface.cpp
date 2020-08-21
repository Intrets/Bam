#include "common.h"

#include "UIOActivityInterface.h"

#include "Colors.h"

int32_t UIOActivityInterface::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (this->base.isValid()) {
		this->base.get()->appendSelectionInfo(gameState, renderInfo, COLORS::GR::HIGHLIGHT);
	}
	return depth;
}

ScreenRectangle UIOActivityInterface::updateSize(ScreenRectangle newScreenRectangle) {
	newScreenRectangle.setHeight(0.0f);
	newScreenRectangle.setWidth(0.0f);

	this->screenRectangle = newScreenRectangle;

	return this->screenRectangle;
}
