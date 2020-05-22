#include "common.h"

#include "UIOBasicWindow.h"
#include"RenderInfo.h"
#include "GameState.h"


UIOBasicWindow::UIOBasicWindow(Handle self) {
	selfHandle = self;
	UIOBase::CallBack onClick = [](GameState& gameState, UIOBase* self) -> bool {
		if (self->contains(gameState.getPlayerCursorScreenSpace())) {
			std::cout << "click anywyere\n";
		}
		return false;
	};
	addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, onClick);
}

ScreenRectangle UIOBasicWindow::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	return screenRectangle;
}

int32_t UIOBasicWindow::addRenderInfo(RenderInfo& renderInfo, int32_t depth) {
	renderInfo.uiRenderInfo.addRectangle(screenRectangle.bot, screenRectangle.top, { 1,0,0,1 }, depth);
	return UIOBase::addRenderInfo(renderInfo, depth + 1);
}
