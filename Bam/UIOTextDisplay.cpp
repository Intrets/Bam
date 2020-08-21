#include "common.h"

#include "UIOTextDisplay.h"
#include "StringHelpers.h"
#include "RenderInfo.h"
#include "GameState.h"
#include "TextRenderer.h"
#include "UIOCallBackParams.h"
#include "TextRenderInfo.h"
#include "Colors.h"

UIOTextDisplay::UIOTextDisplay(Handle self) {
	this->selfHandle = self;
	this->lineWrap = false;
}

UIOTextDisplay::UIOTextDisplay(Handle self, bool lineWrap_) {
	this->selfHandle = self;
	this->lineWrap = lineWrap_;
}

void UIOTextDisplay::translate(glm::vec2 p) {
	this->UIOBase::translate(p);
	if (this->text.cachedRenderInfo.has_value()) {
		this->text.cachedRenderInfo.value().screenRectangle.translate(p);
		this->text.lastScreenRectangle.translate(p);
	}
}

void UIOTextDisplay::setText(std::string text_) {
	this->text.setString(text_);
	this->moveCursor(glm::ivec2(0));
}

void UIOTextDisplay::setText(std::vector<std::string> text_) {
	this->text.setLines(text_);
	this->moveCursor(glm::ivec2(0));
}

void UIOTextDisplay::moveCursor(glm::ivec2 p) {
	this->text.moveCursor(p);
	this->ticksSelected = 0;
}

void UIOTextDisplay::insertText(std::string text_) {
	this->text.insertString(text_);
	this->text.invalidateCache();
	this->moveCursor(glm::ivec2(0));
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

CallBackBindResult UIOTextDisplay::runActiveBinds(State& state) {
	auto result = UIOBase::runActiveBinds(state);
	if (this->active) {
		state.controlState.blockUserInput = true;
	}
	return result;
}

ScreenRectangle UIOTextDisplay::updateSize(ScreenRectangle newScreenRectangle) {
	if (!newScreenRectangle.equals(this->screenRectangle)) {
		this->text.invalidateCache();
	}
	if (this->shrinkToFit) {
		// TODO: font customization/setting in text
		this->text.makeRenderInfo(newScreenRectangle, Fonts::Font::ROBOTO_12, false);

		auto& renderInfo = this->text.cachedRenderInfo.value();
		glm::vec2 screenSize = renderInfo.getRenderedScreenSize();
		newScreenRectangle.setWidth(screenSize.x);
		newScreenRectangle.setHeight(screenSize.y);
		this->screenRectangle = newScreenRectangle;
		return this->screenRectangle;
	}
	else {
		this->screenRectangle = newScreenRectangle;
		return this->screenRectangle;
	}
}

int32_t UIOTextDisplay::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	int32_t ticks = gameState.tick;
	if (this->ticksSelected < 30) {
		ticks = 0;
	}
	depth = this->text.addRenderInfo(this->screenRectangle, renderInfo, Fonts::Font::ROBOTO_12, depth, this->lineWrap, ticks, this->active);

	if (this->active) {
		renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.bot, this->screenRectangle.top, COLORS::UI::FOCUSSED, depth++);

		this->ticksSelected++;
	}

	return depth;
}
