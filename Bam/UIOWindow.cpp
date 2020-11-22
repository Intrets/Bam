#include "common.h"

#include "UIOWindow.h"
#include "UIOConstrainSize.h"
#include "UIOPad.h"
#include "GameState.h"
#include "State.h"
#include "UIOList.h"
#include "RenderInfo.h"

UIOWindow::UIOWindow(Handle self) {
	this->selfHandle = self;
}

int32_t UIOWindow::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	glm::vec2 px = glm::vec2(1.0f) / glm::vec2(this->screenRectangle.getPixelSize());
	if (this->minimized) {
		depth = topBar->addRenderInfo(gameState, renderInfo, depth++);
		renderInfo.uiRenderInfo.addRectangle(this->topBar->getScreenRectangle().getBottomLeft() - px, this->topBar->getScreenRectangle().getTopRight() + 2.0f * px, COLORS::UI::WINDOWBACKGROUND, depth++);
		return depth;
	}
	else {
		depth = this->UIOBaseMulti::addRenderInfo(gameState, renderInfo, depth++);
		renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.getBottomLeft() - px, this->screenRectangle.getTopRight() + 2.0f * px, COLORS::UI::WINDOWBACKGROUND, depth++);
		return depth;
	}
}

CallBackBindResult UIOWindow::runGlobalBinds(State& state) {
	if (this->minimized) {
		return this->topBar->runGlobalBinds(state);
	}
	else {
		return this->UIOBaseMulti::runGlobalBinds(state);
	}
}

CallBackBindResult UIOWindow::runGameWorldBinds(State& state) {
	if (this->minimized) {
		return this->topBar->runGameWorldBinds(state);
	}
	else {
		return this->UIOBaseMulti::runGameWorldBinds(state);
	}
}

void UIOWindow::addElement(UniqueReference<UIOBase, UIOBase> element) {
	assert(this->main == nullptr);
	this->main = element.get();
	this->addElementMulti(std::move(element));
}

void UIOWindow::addElementMulti(UniqueReference<UIOBase, UIOBase> element) {
	this->UIOBaseMulti::addElement(std::move(element));
}

CallBackBindResult UIOWindow::runFocussedBinds(State& state) {
	if (this->minimized) {
		return this->topBar->runFocussedBinds(state);
	}
	else {
		return this->UIOBaseMulti::runFocussedBinds(state);
	}
}

CallBackBindResult UIOWindow::runOnHoverBinds(State& state) {
	if (this->minimized) {
		return this->topBar->runOnHoverBinds(state);
	}
	else {
		return  this->UIOBaseMulti::runOnHoverBinds(state);
	}
}

CallBackBindResult UIOWindow::runActiveBinds(State& state) {
	if (this->minimized) {
		return this->topBar->runActiveBinds(state);
	}
	else {
		return this->UIOBaseMulti::runActiveBinds(state);
	}
}
