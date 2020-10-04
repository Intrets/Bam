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
			ptr->addGlobalBind({ CONTROL::KEY::ACTION0, CONTROL::STATE::PRESSED }, [ptr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				if (!ptr->getScreenRectangle().contains(params.uiState.getCursorPositionScreen())) {
					ptr->deactivate();
					return BIND::RESULT::CONTINUE;
				}
				return BIND::RESULT::CONTINUE;
			});

			ptr->addOnHoverBind({ CONTROL::KEY::ACTION0, CONTROL::STATE::PRESSED }, [ptr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				ptr->activate();
				return BIND::RESULT::CONTINUE | BIND::RESULT::FOCUS | BIND::RESULT::CONSUME;
			});
		}

		void focusable(UIOBase* ptr) {
			ptr->addOnHoverBind({ CONTROL::KEY::ACTION0, CONTROL::STATE::PRESSED }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME | BIND::RESULT::FOCUS;
			});
		}
		void blockWorldBinds(UIOBase* ptr) {
			ptr->addOnHoverBind({ CONTROL::KEY::EVERY_TICK }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				params.controlState.setBlockWorldBinds(true);
				return BIND::RESULT::CONTINUE;
			});
		}
	}

	namespace TextEdit
	{
		void clickSelect(UIOTextDisplay* ptr) {
			ptr->setClickSupport(true);
			ptr->addOnHoverBind({ CONTROL::KEY::ACTION0, CONTROL::STATE::PRESSED },
								[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);

				glm::vec2 click = params.uiState.getCursorPositionScreen() - ptr->getScreenRectangle().getBottomLeft();
				click /= ptr->getScreenRectangle().getAbsSize();
				click = click * 2.0f - 1.0f;
				click += ptr->text.view;

				if (ptr->text.cachedRenderInfo.has_value()) {
					auto maybeIndex = ptr->text.cachedRenderInfo.value().getIndex(click);
					if (maybeIndex.has_value()) {
						ptr->text.selectIndex(maybeIndex.value());
						Locator<Log>::ref().putStreamLine(std::stringstream() << maybeIndex.value());
					}
				}

				return BIND::RESULT::CONTINUE | BIND::RESULT::FOCUS;
			});
		}

		void input(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ CONTROL::KEY::CHAR_BUFFER_CHANGED, CONTROL::STATE::PRESSED },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->insertText(params.controlState.getCharBuffer());
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
			});
		}

		void inputNoLineBreaks(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ CONTROL::KEY::CHAR_BUFFER_CHANGED, CONTROL::STATE::PRESSED },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				auto text = params.controlState.getCharBuffer();
				text.erase(std::remove_if(text.begin(), text.end(), [](auto const c) -> bool
				{
					return c == '\n';
				}));
				ptr->insertText(text);
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
			});
		}

		void backspace(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ CONTROL::KEY::BACKSPACE, CONTROL::STATE::PRESSED | CONTROL::STATE::REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->backspaceChar();
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
			});
		}

		void tab(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ CONTROL::KEY::TAB, CONTROL::STATE::PRESSED | CONTROL::STATE::REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->insertText("  ");
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
			});
		}

		void del(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ CONTROL::KEY::DELETE, CONTROL::STATE::PRESSED | CONTROL::STATE::REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->deleteChar();
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
			});
		}

		void down(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ CONTROL::KEY::TEXT_DOWN, CONTROL::STATE::PRESSED | CONTROL::STATE::REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->moveCursor({ 0,1 });
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
			});
		}

		void up(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ CONTROL::KEY::TEXT_UP, CONTROL::STATE::PRESSED | CONTROL::STATE::REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->moveCursor({ 0,-1 });
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
			});
		}

		void right(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ CONTROL::KEY::TEXT_RIGHT, CONTROL::STATE::PRESSED | CONTROL::STATE::REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->moveCursor({ 1,0 });
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
			});
		}

		void left(UIOTextDisplay* ptr) {
			ptr->addActiveBind({ CONTROL::KEY::TEXT_LEFT, CONTROL::STATE::PRESSED | CONTROL::STATE::REPEAT },
							   [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->moveCursor({ -1,0 });
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
			});
		}

		void viewUp(UIOTextDisplay* ptr) {
			ptr->addOnHoverBind({ CONTROL::KEY::SCROLL_UP, CONTROL::STATE::PRESSED },
								[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->text.moveView({ 0,-1 });
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
			});
		}

		void viewDown(UIOTextDisplay* ptr) {
			ptr->addOnHoverBind({ CONTROL::KEY::SCROLL_DOWN, CONTROL::STATE::PRESSED },
								[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				auto ptr = static_cast<UIOTextDisplay*>(self_);
				ptr->text.moveView({ 0,1 });
				return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
			});
		}

		void wrap(UIOTextDisplay* ptr) {
			ptr->setWrap(true);
		}

		void nowrap(UIOTextDisplay* ptr) {
			ptr->setWrap(false);
		}
	}
}
