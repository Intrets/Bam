#include "common.h"

#include "UIOWindow.h"
#include "UIOConstrainSize.h"
#include "UIOPad.h"
#include "GameState.h"
#include "State.h"
#include "UIOList.h"
#include "UIOConstructButtons.h"

UIOWindow::UIOWindow(Handle self, UniqueReference<UIOBase, UIOBase> main_, std::string title) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	this->selfHandle = self;

	this->main = main_.get();

	auto topBar = refMan->makeUniqueRef<UIOList>(UIOList::DIRECTION::LEFT);
	this->topBarPtr = topBar.get();

	auto [close, closePtr_] = constructButtonWithText("x", glm::ivec2(20, 20), 1);
	//auto closePtr = closePtr_;
	topBar.get()->addElement(std::move(close));

	auto [minimize, minimizePtr_] = constructButtonWithText("_", glm::ivec2(20, 20), 1);
	minimizePtr_->onPress = ([this](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		this->minimized = !this->minimized;
		return BIND_RESULT::CONTINUE;
	});
	//auto minimisePtr = minimizePtr_;
	topBar.get()->addElement(std::move(minimize));

	auto [titleBar, titleBarPtr_] = constructButtonWithText(title);
	auto titleBarPtr = titleBarPtr_;
	topBar.get()->addElement(std::move(titleBar));

	auto topConstrain = refMan->makeUniqueRef<UIOConstrainSize>(std::move(topBar));
	topConstrain.get()->maybeHeight = { UIOSizeType::PX, 20 };
	topConstrain.get()->alignment = UIOConstrainSize::ALIGNMENT::TOP;

	this->addElement(std::move(topConstrain));

	auto mainPad = refMan->makeUniqueRef<UIOPad>(std::move(main_));
	mainPad.get()->top = { UIOSizeType::PX, 20 };
	mainPad.get()->right = { UIOSizeType::PX, 10 };
	mainPad.get()->bottom = { UIOSizeType::PX, 10 };

	this->addElement(std::move(mainPad));

	auto setMoveOrigin = [=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		this->mousePressedPosOffset = this->screenRectangle.getTopLeft() - state.uiState.getCursorPositionScreen();
		return BIND_RESULT::CONTINUE;
	};

	auto move = [=](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (titleBarPtr->down) {
			this->moveTopLeftTo(state.uiState.getCursorPositionScreen() + this->mousePressedPosOffset);
		}
		return BIND_RESULT::CONTINUE;
	};

	auto [rightBar, rightBarPtr_] = constructButton();
	auto rightBarPtr = rightBarPtr_;

	auto paddedRightBar = refMan->makeUniqueRef<UIOPad>(std::move(rightBar));
	paddedRightBar.get()->bottom = UIOSizeType(UIOSizeType::PX, 10);
	paddedRightBar.get()->top = UIOSizeType(UIOSizeType::PX, 20);

	auto rightConstrain = refMan->makeUniqueRef<UIOConstrainSize>(std::move(paddedRightBar));
	rightConstrain.get()->maybeWidth = { UIOSizeType::PX, 10 };
	rightConstrain.get()->alignment = UIOConstrainSize::ALIGNMENT::RIGHT;

	addElement(std::move(rightConstrain));

	auto [bottomBar, bottomBarPtr_] = constructButton();
	auto bottomBarPtr = bottomBarPtr_;

	auto paddedbottomBar = refMan->makeUniqueRef<UIOPad>(std::move(bottomBar));
	paddedbottomBar.get()->right = UIOSizeType(UIOSizeType::PX, 10);

	auto bottomConstrain = refMan->makeUniqueRef<UIOConstrainSize>(std::move(paddedbottomBar));
	bottomConstrain.get()->maybeHeight = { UIOSizeType::PX, 10 };
	bottomConstrain.get()->alignment = UIOConstrainSize::ALIGNMENT::BOTTOMLEFT;

	addElement(std::move(bottomConstrain));


	auto [cornerBar, cornerBarPtr_] = constructButton();
	auto cornerBarPtr = cornerBarPtr_;

	auto cornerConstrain = refMan->makeUniqueRef<UIOConstrainSize>(std::move(cornerBar));
	cornerConstrain.get()->maybeHeight = { UIOSizeType::PX, 10 };
	cornerConstrain.get()->maybeWidth = { UIOSizeType::PX, 10 };
	cornerConstrain.get()->alignment = UIOConstrainSize::ALIGNMENT::BOTTOMRIGHT;

	addElement(std::move(cornerConstrain));


	auto scaleVertical = [this, bottomBarPtr](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (bottomBarPtr->down) {
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

	auto scaleHorizontal = [this, rightBarPtr](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (rightBarPtr->down) {
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

	auto scaleDiagonal = [this, cornerBarPtr](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (cornerBarPtr->down) {
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

	addFocussedBind({ ControlState::CONTROLS::MOUSE_POS_CHANGED, ControlState::CONTROLSTATE_PRESSED }, scaleVertical);
	addFocussedBind({ ControlState::CONTROLS::MOUSE_POS_CHANGED, ControlState::CONTROLSTATE_PRESSED }, scaleHorizontal);
	addFocussedBind({ ControlState::CONTROLS::MOUSE_POS_CHANGED, ControlState::CONTROLSTATE_PRESSED }, scaleDiagonal);

	this->topBarPtr->addFocussedBind({ ControlState::CONTROLS::MOUSE_POS_CHANGED, ControlState::CONTROLSTATE_PRESSED }, move);
	this->topBarPtr->addGlobalBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED }, setMoveOrigin);
}

ScreenRectangle UIOWindow::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

	for (auto& element : this->elements) {
		element.get()->updateSize(this->screenRectangle);
	}
	return this->screenRectangle;
}

int32_t UIOWindow::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (minimized) {
		return topBarPtr->addRenderInfo(gameState, renderInfo, depth + 1);
	}
	else {
		return this->UIOBase::addRenderInfo(gameState, renderInfo, depth + 1);
	}
}

CallBackBindResult UIOWindow::runGlobalBinds(State& state) {
	if (minimized) {
		return this->topBarPtr->runGlobalBinds(state);
	}
	else {
		return this->UIOBase::runGlobalBinds(state);
	}
}

CallBackBindResult UIOWindow::runFocussedBinds(State& state) {
	if (minimized) {
		return this->topBarPtr->runFocussedBinds(state);
	}
	else {
		return this->UIOBase::runFocussedBinds(state);
	}
}
