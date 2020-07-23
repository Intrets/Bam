#include "common.h"

#include "UIOWindow.h"
#include "UIOConstrainSize.h"
#include "UIOPad.h"
#include "GameState.h"
#include "State.h"

UIOWindow::UIOWindow(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	this->selfHandle = self;

	this->main = main_.get();

	auto mainPad = refMan->makeUniqueRef<UIOPad>(std::move(main_));
	mainPad.get()->top = { UIOSizeType::PX, 30 };
	mainPad.get()->right = { UIOSizeType::PX, 20 };
	mainPad.get()->bottom = { UIOSizeType::PX, 20 };

	addElement(std::move(mainPad));

	auto top = refMan->makeUniqueRef<UIOButton>();
	this->topBar = top.get();

	auto topConstrain = refMan->makeUniqueRef<UIOConstrainSize>(std::move(top));
	topConstrain.get()->maybeHeight = { UIOSizeType::PX, 30 };
	topConstrain.get()->alignment = CONSTRAIN_ALIGNMENT::TOP;

	addElement(std::move(topConstrain));

	auto setMoveOrigin = [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		this->mousePressedPosOffset = this->screenRectangle.getTopLeft() - state.uiState.getCursorPositionScreen();
		return BIND_RESULT::CONTINUE;
	};

	auto move = [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (this->topBar->down) {
			this->moveTopLeftTo(state.uiState.getCursorPositionScreen() + this->mousePressedPosOffset);
		}
		return BIND_RESULT::CONTINUE;
	};

	{
		auto br = refMan->makeUniqueRef<UIOButton>();
		this->bottomRightBar = br.get();

		auto brConstrain = refMan->makeUniqueRef<UIOConstrainSize>(std::move(br));
		brConstrain.get()->maybeHeight = { UIOSizeType::PX, 20 };
		brConstrain.get()->maybeWidth = { UIOSizeType::PX, 20 };
		brConstrain.get()->alignment = CONSTRAIN_ALIGNMENT::BOTTOMRIGHT;

		addElement(std::move(brConstrain));
	}

	auto b = refMan->makeUniqueRef<UIOButton>();
	this->bottomBar = b.get();

	auto botConstrain = refMan->makeUniqueRef<UIOConstrainSize>(std::move(b));
	botConstrain.get()->maybeHeight = { UIOSizeType::PX, 20 };
	botConstrain.get()->alignment = CONSTRAIN_ALIGNMENT::BOTTOMRIGHT;

	addElement(std::move(botConstrain));

	auto r = refMan->makeUniqueRef<UIOButton>();
	this->rightBar = r.get();

	auto rightConstrain = refMan->makeUniqueRef<UIOConstrainSize>(std::move(r));
	rightConstrain.get()->maybeWidth = { UIOSizeType::PX, 20 };
	rightConstrain.get()->alignment = CONSTRAIN_ALIGNMENT::BOTTOMRIGHT;

	addElement(std::move(rightConstrain));

	auto scaleVertical = [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (this->bottomBar->down) {
			auto bottomRight = this->screenRectangle.getBottomRight();
			bottomRight.y = state.uiState.getCursorPositionScreen().y;
			if (this->screenRectangle.top.y - bottomRight.y < 0.2f) {
				bottomRight.y = this->screenRectangle.top.y - 0.2f;
			}
			this->screenRectangle.setBottomRight(bottomRight);
			this->updateSize(this->screenRectangle);
		}
		return BIND_RESULT::CONTINUE;
	};

	auto scaleHorizontal = [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (this->rightBar->down) {
			auto bottomRight = this->screenRectangle.getBottomRight();
			bottomRight.x = state.uiState.getCursorPositionScreen().x;
			if (bottomRight.x - this->screenRectangle.bot.x < 0.2f) {
				bottomRight.x = this->screenRectangle.bot.x + 0.2f;
			}
			this->screenRectangle.setBottomRight(bottomRight);
			this->updateSize(this->screenRectangle);
		}
		return BIND_RESULT::CONTINUE;
	};

	auto scaleDiagonal = [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (this->bottomRightBar->down) {
			auto bottomRight = state.uiState.getCursorPositionScreen();
			if (bottomRight.x - this->screenRectangle.bot.x < 0.2f) {
				bottomRight.x = this->screenRectangle.bot.x + 0.2f;
			}
			if (this->screenRectangle.top.y - bottomRight.y < 0.2f) {
				bottomRight.y = this->screenRectangle.top.y - 0.2f;
			}
			this->screenRectangle.setBottomRight(bottomRight);
			this->updateSize(this->screenRectangle);
		}
		return BIND_RESULT::CONTINUE;
	};

	addGlobalBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED }, scaleVertical);
	addGlobalBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED }, scaleHorizontal);
	addGlobalBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED }, move);
	addGlobalBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED }, scaleDiagonal);
	addGlobalBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, setMoveOrigin);
}

ScreenRectangle UIOWindow::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

	for (auto& element : this->elements) {
		element.get()->updateSize(this->screenRectangle);
	}
	return this->screenRectangle;
}

int32_t UIOWindow::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	return this->UIOBase::addRenderInfo(gameState, renderInfo, depth + 1);
}
