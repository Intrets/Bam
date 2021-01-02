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

CallBackBindResult UIOWindow::runGlobalBinds(PlayerState& playerState) {
	if (this->minimized) {
		return this->topBar.get()->runGlobalBinds(playerState);
	}
	else {
		return this->UIOBaseMulti::runGlobalBinds(playerState);
	}
}

CallBackBindResult UIOWindow::runGameWorldBinds(PlayerState& playerState) {
	if (this->minimized) {
		return this->topBar.get()->runGameWorldBinds(playerState);
	}
	else {
		return this->UIOBaseMulti::runGameWorldBinds(playerState);
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

CallBackBindResult UIOWindow::runFocussedBinds(PlayerState& playerState) {
	if (this->minimized) {
		return this->topBar.get()->runFocussedBinds(playerState);
	}
	else {
		return this->UIOBaseMulti::runFocussedBinds(playerState);
	}
}

CallBackBindResult UIOWindow::runOnHoverBinds(PlayerState& playerState) {
	if (this->minimized) {
		return this->topBar.get()->runOnHoverBinds(playerState);
	}
	else {
		return  this->UIOBaseMulti::runOnHoverBinds(playerState);
	}
}

CallBackBindResult UIOWindow::runActiveBinds(PlayerState& playerState) {
	if (this->minimized) {
		return this->topBar.get()->runActiveBinds(playerState);
	}
	else {
		return this->UIOBaseMulti::runActiveBinds(playerState);
	}
}
