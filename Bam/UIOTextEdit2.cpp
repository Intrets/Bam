#include "common.h"

#include "UIOTextEdit2.h"

#include "UIOCallBackParams.h"

UniqueReference<UIOBase, UIOTextDisplay> constructTextEdit(std::string text) {
	auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>();
	auto ptr = res.get();
	ptr->text.addLine(text);

	ptr->addFocussedBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED },
						 [=](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		if (!ptr->contains(params.uiState.getCursorPositionScreen())) {
			return BIND_RESULT::CONTINUE;
		}

		glm::vec2 click = params.uiState.getCursorPositionScreen() - ptr->screenRectangle.getBottomLeft();
		click /= ptr->screenRectangle.size();
		click = click * 2.0f - 1.0f;
		click += ptr->text.view;

		if (ptr->text.cachedRenderInfo.has_value()) {
			auto maybeIndex = ptr->text.cachedRenderInfo.value().getIndex(click);
			if (maybeIndex.has_value()) {
				//ptr->text.cursorIndex = maybeIndex.value();
				ptr->text.selectIndex(maybeIndex.value());
				Locator<Log>::ref() << Log::OPEN{} << maybeIndex.value() << "\n" << Log::CLOSE{};
			}
		}

		return BIND_RESULT::CONTINUE | BIND_RESULT::FOCUS | BIND_RESULT::CONSUME;
	});

	ptr->addFocussedBind({ ControlState::CONTROLS::TEST_UP, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->moveCursor({ 0,-1 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	ptr->addFocussedBind({ ControlState::CONTROLS::TEST_DOWN, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->moveCursor({ 0,1 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	ptr->addFocussedBind({ ControlState::CONTROLS::TEST_LEFT, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->moveCursor({ -1,0 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	ptr->addFocussedBind({ ControlState::CONTROLS::TEST_RIGHT, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->moveCursor({ 1,0 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	ptr->addFocussedBind({ ControlState::CONTROLS::CHAR_BUFFER_CHANGED, ControlState::CONTROLSTATE_PRESSED },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->insertText(state.controlState.getCharBuffer());
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	ptr->addFocussedBind({ ControlState::CONTROLS::BACKSPACE, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->backspaceChar();
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	ptr->addFocussedBind({ ControlState::CONTROLS::DELETE, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->deleteChar();
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	ptr->addFocussedBind({ ControlState::CONTROLS::TAB, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->insertText("    ");
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	return std::move(res);
}
