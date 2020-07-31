#include "common.h"

#include "UIOTextEdit2.h"

#include "UIOCallBackParams.h"

UniqueReference<UIOBase, UIOTextDisplay> constructTextEdit() {
	auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>();
	auto ptr = res.get();

	ptr->text.addLine("11234567890");
	ptr->text.addLine("21234567890");
	ptr->text.addLine("31234567890");
	ptr->text.addLine("41234567890");
	ptr->text.addLine("51234567890");
	ptr->text.addLine("61234567890");
	ptr->text.addLine("71234567890");

	ptr->addFocussedBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED },
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

	ptr->addFocussedBind({ CONTROLS::TEST_UP, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->moveCursor({ 0,-1 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	ptr->addFocussedBind({ CONTROLS::TEST_DOWN, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->moveCursor({ 0,1 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	ptr->addFocussedBind({ CONTROLS::TEST_LEFT, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->moveCursor({ -1,0 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	ptr->addFocussedBind({ CONTROLS::TEST_RIGHT, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->moveCursor({ 1,0 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	ptr->addFocussedBind({ CONTROLS::CHAR_BUFFER_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->insertText(state.controlState.getCharBuffer());
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	ptr->addFocussedBind({ CONTROLS::BACKSPACE, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->backspaceChar();
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	ptr->addFocussedBind({ CONTROLS::DELETE, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->deleteChar();
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	ptr->addFocussedBind({ CONTROLS::TAB, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		ptr->insertText("    ");
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	return std::move(res);
}
