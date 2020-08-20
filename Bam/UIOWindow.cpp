#include "common.h"

#include "UIOWindow.h"
#include "UIOConstrainSize.h"
#include "UIOPad.h"
#include "GameState.h"
#include "State.h"
#include "UIOList.h"
#include "UIOConstructButtons.h"

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
