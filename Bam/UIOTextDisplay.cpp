#include "common.h"

#include "UIOTextDisplay.h"
#include "StringHelpers.h"
#include "RenderInfo.h"
#include "GameState.h"
#include "TextRenderer.h"
#include "UIOCallBackParams.h"
#include "TextRenderInfo.h"
#include "Colors.h"
#include "Enums.h"

UIOTextDisplay::UIOTextDisplay(Handle self) {
	this->selfHandle = self;
	this->lineWrap = false;
}

UIOTextDisplay::UIOTextDisplay(Handle self, bool lineWrap_) {
	this->selfHandle = self;
	this->lineWrap = lineWrap_;
}

void UIOTextDisplay::translate(glm::vec2 p) {
	this->UIOBaseEnd::translate(p);
	if (this->text.cachedRenderInfo.has_value()) {
		this->text.cachedRenderInfo.value().screenRectangle.translate(p);
		this->text.lastScreenRectangle.translate(p);
	}
}

void UIOTextDisplay::setText(std::string const& text_) {
	this->text.setString(text_);
	this->moveCursor(glm::ivec2(0));
}

void UIOTextDisplay::setText(std::vector<std::string> const& text_) {
	this->text.setLines(text_);
	this->moveCursor(glm::ivec2(0));
}

void UIOTextDisplay::moveCursor(glm::ivec2 p) {
	this->text.moveCursor(p);
	this->ticksSelected = 0;
}

void UIOTextDisplay::setCursor(glm::ivec2 p) {
	this->text.setCursor(p);
	this->ticksSelected = 0;
}

void UIOTextDisplay::moveStartWordForward() {
	this->text.moveStartWordForward();
	this->ticksSelected = 0;
}

void UIOTextDisplay::moveStartWordBackward() {
	this->text.moveStartWordBackward();
	this->ticksSelected = 0;
}

void UIOTextDisplay::moveEndWord() {
	this->text.moveEndWord();
	this->ticksSelected = 0;
}

bool UIOTextDisplay::yank(ControlState& controlState) {
	if (this->mode == UIOTEXTDISPLAY::MODE::NORMAL) {
		controlState.setClipboard(this->text.getSelectedLine());
		return true;
	}
	return false;
}

bool UIOTextDisplay::paste(ControlState& controlState) {
	if (this->mode == UIOTEXTDISPLAY::MODE::NORMAL) {
		this->text.insertString(controlState.getClipboard());
		return true;
	}
	return false;
}

void UIOTextDisplay::insertText(std::string text_) {
	if (this->mode == UIOTEXTDISPLAY::MODE::INSERT) {
		this->text.insertString(text_);
		this->text.invalidateCache();
		this->moveCursor(glm::ivec2(0));
	}
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

void UIOTextDisplay::setMode(UIOTEXTDISPLAY::MODE mode_) {
	this->mode = mode_;
	this->ticksSelected = 0;
}

void UIOTextDisplay::setShrinkToFit(bool b) {
	this->shrinkToFit = b;
}

void UIOTextDisplay::setClickSupport(bool b) {
	this->clickSupport = b;
}

void UIOTextDisplay::setWrap(bool b) {
	this->lineWrap = b;
}

ScreenRectangle UIOTextDisplay::updateSize(ScreenRectangle newScreenRectangle) {
	if (!newScreenRectangle.equals(this->screenRectangle)) {
		this->text.invalidateCache();
	}
	if (this->shrinkToFit) {
		// TODO: font customization/setting in text
		if (!this->text.cachedRenderInfo.has_value()) {
			this->text.makeRenderInfo(newScreenRectangle, FONTS::FONT::ROBOTO_12, this->lineWrap, this->clickSupport);
		}

		auto& renderInfo = this->text.cachedRenderInfo.value();
		glm::vec2 screenSize = renderInfo.getRenderedScreenSize();
		screenSize = glm::min(newScreenRectangle.getAbsSize(), screenSize);
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

	CURSOR::TYPE cursorType = CURSOR::TYPE::BLOCK;

	switch (this->mode) {
		case UIOTEXTDISPLAY::MODE::INSERT:
			cursorType = CURSOR::TYPE::LINE;
			break;
		case UIOTEXTDISPLAY::MODE::NORMAL:
			cursorType = CURSOR::TYPE::BLOCK;
			break;
		case UIOTEXTDISPLAY::MODE::VISUAL:
			cursorType = CURSOR::TYPE::BLOCK;
			break;
		default:
			assert(0);
			break;
	}

	depth = this->text.addRenderInfo(this->screenRectangle, renderInfo, FONTS::FONT::ROBOTO_12, depth, this->lineWrap, ticks, this->active, this->clickSupport, cursorType);

	if (this->active) {
		renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.getBottomLeft(), this->screenRectangle.getTopRight(), COLORS::UI::FOCUSSED, depth++);

		this->ticksSelected++;
	}

	return depth;
}
