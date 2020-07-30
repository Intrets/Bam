#include "common.h"

#include "UIOFinalTextDisplay.h"
#include "RenderInfo.h"

ScreenRectangle UIOFinalTextDisplay::updateSize(ScreenRectangle newScreenRectangle) {
	this->text.makeRenderInfo(screenRectangle, Fonts::Font::ROBOTO_12, false);
	auto& renderInfo = this->text.cachedRenderInfo.value();
	glm::vec2 screenSize = renderInfo.getRenderedScreenSize();
	newScreenRectangle.setWidth(screenSize.x);
	newScreenRectangle.setHeight(screenSize.y);
	//this->text.addRenderInfo(this->screenRectangle, )
	return ScreenRectangle();
}

int32_t UIOFinalTextDisplay::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	depth = this->text.addRenderInfo(this->screenRectangle, renderInfo, Fonts::Font::ROBOTO_12, depth, true);
	return depth;
}

UIOFinalTextDisplay::UIOFinalTextDisplay(Handle self, std::string text_) {
	this->selfHandle = self;
	this->text.addLine(text_);
}
