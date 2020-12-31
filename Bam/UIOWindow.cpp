#include "common.h"

#include "UIOWindow.h"
#include "UIOConstrainSize.h"
#include "UIOPad.h"
#include "GameState.h"
#include "PlayerState.h"
#include "UIOList.h"
#include "RenderInfo.h"

UIOWindow::UIOWindow(Handle self) {
	this->selfHandle = self;
}

int32_t UIOWindow::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	glm::vec2 px = glm::vec2(1.0f) / glm::vec2(this->screenRectangle.getPixelSize());
	if (this->minimized) {
		depth = this->topBar.get()->addRenderInfo(gameState, renderInfo, depth++);
		renderInfo.uiRenderInfo.addRectangle(this->topBar.get()->getScreenRectangle().getBottomLeft() - px, this->topBar.get()->getScreenRectangle().getTopRight() + 2.0f * px, COLORS::UI::WINDOWBACKGROUND, depth++);
		return depth;
	}
	else {
		depth = this->UIOBaseMulti::addRenderInfo(gameState, renderInfo, depth++);
		renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.getBottomLeft() - px, this->screenRectangle.getTopRight() + 2.0f * px, COLORS::UI::WINDOWBACKGROUND, depth++);
		return depth;
	}
}

CallBackBindResult UIOWindow::runGlobalBinds(PlayerState& state) {
	if (this->minimized) {
		return this->topBar.get()->runGlobalBinds(state);
	}
	else {
		return this->UIOBaseMulti::runGlobalBinds(state);
	}
}

CallBackBindResult UIOWindow::runGameWorldBinds(PlayerState& state) {
	if (this->minimized) {
		return this->topBar.get()->runGameWorldBinds(state);
	}
	else {
		return this->UIOBaseMulti::runGameWorldBinds(state);
	}
}

void UIOWindow::addElement(UniqueReference<UIOBase, UIOBase> element) {
	assert(this->main == nullptr);
	this->main = element;
	this->addElementMulti(std::move(element));
}

void UIOWindow::addElementMulti(UniqueReference<UIOBase, UIOBase> element) {
	this->UIOBaseMulti::addElement(std::move(element));
}

CallBackBindResult UIOWindow::runFocussedBinds(PlayerState& state) {
	if (this->minimized) {
		return this->topBar.get()->runFocussedBinds(state);
	}
	else {
		return this->UIOBaseMulti::runFocussedBinds(state);
	}
}

CallBackBindResult UIOWindow::runOnHoverBinds(PlayerState& state) {
	if (this->minimized) {
		return this->topBar.get()->runOnHoverBinds(state);
	}
	else {
		return  this->UIOBaseMulti::runOnHoverBinds(state);
	}
}

CallBackBindResult UIOWindow::runActiveBinds(PlayerState& state) {
	if (this->minimized) {
		return this->topBar.get()->runActiveBinds(state);
	}
	else {
		return this->UIOBaseMulti::runActiveBinds(state);
	}
}
