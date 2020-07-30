#include "common.h"

#include "UIOTextEdit.h"
#include "RenderInfo.h"
#include "TextRenderer.h"
#include "GameState.h"
#include "StringHelpers.h"
#include "State.h"
#include "UIOCallBackParams.h"

UIOTextEdit::UIOTextEdit(Handle self) {
	this->selfHandle = self;
	this->text.addLine("11234567890");
	this->text.addLine("21234567890");
	this->text.addLine("31234567890");
	this->text.addLine("41234567890");
	this->text.addLine("51234567890");
	this->text.addLine("61234567890");
	this->text.addLine("71234567890");

	addGlobalBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED },
				  [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		this->focus = this->contains(params.uiState.getCursorPositionScreen());
		if (this->focus) {
			glm::vec2 click = params.uiState.getCursorPositionScreen() - this->screenRectangle.getBottomLeft();
			click /= this->screenRectangle.size();
			click = click*2.0f - 1.0f;
			click += this->text.view;

			if (this->text.cachedRenderInfo.has_value()) {
				auto maybeIndex = this->text.cachedRenderInfo.value().getIndex(click);
				if (maybeIndex.has_value()) {
					//this->text.cursorIndex = maybeIndex.value();
					this->text.selectIndex(maybeIndex.value());
					Locator<Log>::ref() << Log::OPEN{} << maybeIndex.value() << "\n" << Log::CLOSE{};
				}
			}
			
			return BIND_RESULT::CONTINUE | BIND_RESULT::FOCUS | BIND_RESULT::CONSUME;
		}
		else {
			return BIND_RESULT::CONTINUE;
		}
	});

	addFocussedBind({ CONTROLS::TEST_UP, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		this->moveCursor({ 0,-1 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	addFocussedBind({ CONTROLS::TEST_DOWN, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		this->moveCursor({ 0,1 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	addFocussedBind({ CONTROLS::TEST_LEFT, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		this->moveCursor({ -1,0 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	addFocussedBind({ CONTROLS::TEST_RIGHT, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		this->moveCursor({ 1,0 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	addFocussedBind({ CONTROLS::CHAR_BUFFER_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		insertText(state.controlState.getCharBuffer());
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	addFocussedBind({ CONTROLS::BACKSPACE, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		backspaceChar();
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	addFocussedBind({ CONTROLS::DELETE, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		deleteChar();
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	addFocussedBind({ CONTROLS::TAB, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		insertText("    ");
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
}

void UIOTextEdit::translate(glm::vec2 p) {
	this->UIOBase::translate(p);
	if (this->text.cachedRenderInfo.has_value()) {
		this->text.cachedRenderInfo.value().screenRectangle.translate(p);
	}
}

ScreenRectangle UIOTextEdit::updateSize(ScreenRectangle newScreenRectangle) {
	if (!this->screenRectangle.equals(newScreenRectangle)) {
		this->text.invalidateCache();
	}
	this->screenRectangle = newScreenRectangle;
	return this->screenRectangle;
}

int32_t UIOTextEdit::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	depth = this->text.addRenderInfo(this->screenRectangle, renderInfo, Fonts::Font::ROBOTO_12, depth, true);
	renderInfo.uiRenderInfo.addRectangle(screenRectangle.bot, screenRectangle.top, { 0.9f, 0.9f, 0.9f, 1.0f }, depth++);

	return depth;
}

void UIOTextEdit::moveCursor(glm::ivec2 p) {
	this->text.moveCursor(p);
}

void UIOTextEdit::insertText(std::string text_) {
	this->text.insertString(text_);
	this->text.invalidateCache();
	moveCursor(glm::ivec2(0));
}

void UIOTextEdit::backspaceChar() {
	if (this->text.backspaceChar()) {
		this->text.invalidateCache();
		moveCursor(glm::ivec2(0));
	}
}

void UIOTextEdit::deleteChar() {
	if (this->text.deleteChar()) {
		this->text.invalidateCache();
		moveCursor(glm::ivec2(0));
	}
}
