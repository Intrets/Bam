#include "common.h"

#include "UIOTextDisplay.h"
#include "StringHelpers.h"
#include "RenderInfo.h"
#include "GameState.h"
#include "TextRenderer.h"
#include "UIOCallBackParams.h"
#include "TextRenderInfo.h"

UIOTextDisplay::UIOTextDisplay(Handle self) {
	this->selfHandle = self;
}

//void UIOTextDisplay::setText(std::string text_) {
//	this->text.setText(text_);
//}
//
//void UIOTextDisplay::addLine(std::string text_) {
//	this->text.addLine(text_);
//}

ScreenRectangle UIOTextDisplay::updateSize(ScreenRectangle newScreenRectangle) {
	if (!newScreenRectangle.equals(this->screenRectangle)) {
		this->text.invalidateCache();
	}
	this->screenRectangle = newScreenRectangle;

	return this->screenRectangle;
}

int32_t UIOTextDisplay::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	glm::vec4 color;
	glm::vec2 top = this->screenRectangle.getTopLeft();
	glm::vec2 bot;

	depth = this->text.addRenderInfo(this->screenRectangle, renderInfo, Fonts::Font::ROBOTO_12, depth, true);

	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.bot, this->screenRectangle.top, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), depth++);

	return depth;
}
