#include "common.h"

#include "UIOBasicWindow.h"
#include"RenderInfo.h"

UIOBasicWindow::UIOBasicWindow(Handle self) {
	selfHandle = self;
}

ScreenRectangle UIOBasicWindow::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	return screenRectangle;
}

int32_t UIOBasicWindow::addRenderInfo(RenderInfo& renderInfo, int32_t depth) {
	renderInfo.uiRenderInfo.addRectangle(screenRectangle.bot, screenRectangle.top, { 1,0,0,1 }, depth);
	return UIOBase::addRenderInfo(renderInfo, depth + 1);
}
