#include "common.h"

#include "UIOBinds.h"
#include "UIOBase.h"
#include "UIOCallBackParams.h"
#include "UIOTextDisplay.h"

namespace UIOBinds
{
	namespace Base
	{
		void activatable(UIOBase* ptr) {
			ptr->addGlobalBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED }, [ptr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				if (!ptr->getScreenRectangle().contains(params.uiState.getCursorPositionScreen())) {
					ptr->deactivate();
					return BIND_RESULT::CONTINUE;
				}
				return BIND_RESULT::CONTINUE;
			});

			ptr->addOnHoverBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED }, [ptr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				ptr->activate();
				return BIND_RESULT::CONTINUE | BIND_RESULT::FOCUS;
			});
		}
	}

	namespace TextEdit
	{
		void clickSelect(UIOTextDisplay* ptr) {
			ptr->addOnHoverBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED },
								[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);

				glm::vec2 click = params.uiState.getCursorPositionScreen() - ptr->getScreenRectangle().getBottomLeft();
				click /= ptr->getScreenRectangle().size();
				click = click * 2.0f - 1.0f;
				click += ptr->text.view;

				if (ptr->text.cachedRenderInfo.has_value()) {
					auto maybeIndex = ptr->text.cachedRenderInfo.value().getIndex(click);
					if (maybeIndex.has_value()) {
						ptr->text.selectIndex(maybeIndex.value());
						Locator<Log>::ref().putStreamLine(std::stringstream() << maybeIndex.value());
					}
				}

				return BIND_RESULT::CONTINUE | BIND_RESULT::FOCUS ;
			});
		}

		void input(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ ControlState::CONTROLS::CHAR_BUFFER_CHANGED, ControlState::CONTROLSTATE_PRESSED },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->insertText(params.controlState.getCharBuffer());
				return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
			});
		}

		void inputNoLineBreaks(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ ControlState::CONTROLS::CHAR_BUFFER_CHANGED, ControlState::CONTROLSTATE_PRESSED },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				auto text = params.controlState.getCharBuffer();
				text.erase(std::remove_if(text.begin(), text.end(), [](auto const c) -> bool
				{
					return c == '\n';
				}));
				ptr->insertText(text);
				return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
			});
		}

		void backspace(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ ControlState::CONTROLS::BACKSPACE, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->backspaceChar();
				return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
			});
		}

		void tab(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ ControlState::CONTROLS::TAB, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->insertText("    ");
				return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
			});
		}

		void del(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ ControlState::CONTROLS::DELETE, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->deleteChar();
				return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
			});
		}

		void down(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ ControlState::CONTROLS::TEST_DOWN, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->moveCursor({ 0,1 });
				return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
			});
		}

		void up(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ ControlState::CONTROLS::TEST_UP, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->moveCursor({ 0,-1 });
				return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
			});
		}

		void right(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ ControlState::CONTROLS::TEST_RIGHT, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->moveCursor({ 1,0 });
				return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
			});
		}

		void left(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ ControlState::CONTROLS::TEST_LEFT, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->moveCursor({ -1,0 });
				return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
			});
		}

		void viewUp(UIOTextDisplay* ptr) {
			ptr->addOnHoverBind({ ControlState::CONTROLS::SCROLL_UP, ControlState::CONTROLSTATE_PRESSED },
								[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->text.moveView({ 0,-1 });
				return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
			});
		}

		void viewDown(UIOTextDisplay* ptr) {
			ptr->addOnHoverBind({ ControlState::CONTROLS::SCROLL_DOWN, ControlState::CONTROLSTATE_PRESSED },
								[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->text.moveView({ 0,1 });
				return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
			});
		}
	}
}
