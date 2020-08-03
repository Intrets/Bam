#include "common.h"

#include "UIOSimpleTextDisplay.h"
#include "RenderInfo.h"

ScreenRectangle UIOSimpleTextDisplay::updateSize(ScreenRectangle newScreenRectangle) {
	this->text.makeRenderInfo(newScreenRectangle, Fonts::Font::ROBOTO_12, false);
	auto& renderInfo = this->text.cachedRenderInfo.value();
	glm::vec2 screenSize = renderInfo.getRenderedScreenSize();
	newScreenRectangle.setWidth(screenSize.x);
	newScreenRectangle.setHeight(screenSize.y);
	this->screenRectangle = newScreenRectangle;
	return this->screenRectangle;
}

int32_t UIOSimpleTextDisplay::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	depth = this->text.addRenderInfo(this->screenRectangle, renderInfo, Fonts::Font::ROBOTO_12, depth, true);
	return depth;
}

UIOSimpleTextDisplay::UIOSimpleTextDisplay(Handle self, std::string text_) {
	this->selfHandle = self;
	this->text.addString(text_);
	this->text.cursorIndex = -1;
}

void UIOSimpleTextDisplay::translate(glm::vec2 p) {
	this->UIOBase::translate(p);
	if (this->text.cachedRenderInfo.has_value()) {
		this->text.cachedRenderInfo.value().screenRectangle.translate(p);
		this->text.lastScreenRectangle.translate(p);
	}
}
