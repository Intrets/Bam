#include "common.h"

#include "UIOBasicWindow.h"
#include"RenderInfo.h"
#include "GameState.h"


UIOBasicWindow::UIOBasicWindow(Handle self) {
	this->selfHandle = self;
}

ScreenRectangle UIOBasicWindow::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;
	return this->screenRectangle;
}

int32_t UIOBasicWindow::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.bot, this->screenRectangle.top, { 1,0,0,1 }, depth);
	return UIOBase::addRenderInfo(gameState, renderInfo, depth + 1);
}
