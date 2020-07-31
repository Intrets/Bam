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

void UIOTextDisplay::translate(glm::vec2 p) {
	this->UIOBase::translate(p);
	if (this->text.cachedRenderInfo.has_value()) {
		this->text.cachedRenderInfo.value().screenRectangle.translate(p);
	}
}

void UIOTextDisplay::moveCursor(glm::ivec2 p) {
	this->text.moveCursor(p);
}

void UIOTextDisplay::insertText(std::string text_) {
	this->text.insertString(text_);
	this->text.invalidateCache();
	moveCursor(glm::ivec2(0));
}

void UIOTextDisplay::backspaceChar() {
	if (this->text.backspaceChar()) {
		this->text.invalidateCache();
		moveCursor(glm::ivec2(0));
	}
}

void UIOTextDisplay::deleteChar() {
	if (this->text.deleteChar()) {
		this->text.invalidateCache();
		moveCursor(glm::ivec2(0));
	}
}

ScreenRectangle UIOTextDisplay::updateSize(ScreenRectangle newScreenRectangle) {
	if (!newScreenRectangle.equals(this->screenRectangle)) {
		this->text.invalidateCache();
	}
	this->screenRectangle = newScreenRectangle;
	return this->screenRectangle;
}

int32_t UIOTextDisplay::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	depth = this->text.addRenderInfo(this->screenRectangle, renderInfo, Fonts::Font::ROBOTO_12, depth, true);

	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.bot, this->screenRectangle.top, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), depth++);

	return depth;
}
