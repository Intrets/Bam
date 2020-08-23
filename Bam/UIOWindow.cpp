#include "common.h"

#include "UIOWindow.h"
#include "UIOConstrainSize.h"
#include "UIOPad.h"
#include "GameState.h"
#include "State.h"
#include "UIOList.h"
#include "RenderInfo.h"

UIOWindow::UIOWindow(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	this->selfHandle = self;
	this->main = main_.get();
	this->addElement(std::move(main_));
}

UIOWindow::UIOWindow(Handle self) {
	this->selfHandle = self;
}

ScreenRectangle UIOWindow::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

	for (auto& element : this->elements) {
		element.get()->updateSize(this->screenRectangle);
	}
	return this->screenRectangle;
}

int32_t UIOWindow::addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) {
	glm::vec2 px = glm::vec2(1.0f) / glm::vec2(this->screenRectangle.getPixelSize());
	if (minimized) {
		depth = topBarPtr->addRenderInfo(gameState, renderInfo, depth++);
		renderInfo.uiRenderInfo.addRectangle(this->topBarPtr->getScreenRectangle().getBottomLeft() - px, this->topBarPtr->getScreenRectangle().getTopRight() + 2.0f * px, COLORS::UI::WINDOWBACKGROUND, depth++);
		return depth;
	}
	else {
		depth = this->UIOBase::addRenderInfo(gameState, renderInfo, depth++);
		renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.getBottomLeft() - px, this->screenRectangle.getTopRight() + 2.0f * px, COLORS::UI::WINDOWBACKGROUND, depth++);
		return depth;
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

CallBackBindResult UIOWindow::runOnHoverBinds(State& state) {
	auto result = UIOBase::runOnHoverBinds(state);

	if (this->screenRectangle.contains(state.uiState.getCursorPositionScreen()) && state.controlState.activated({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED })) {
		state.controlState.consumeControl(ControlState::CONTROLS::ACTION0);
	}

	return result;
}
