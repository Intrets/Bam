#include "common.h"

#include "UIOColoredBackground.h"
#include "RenderInfo.h"

UIOColoredBackground::UIOColoredBackground(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	this->main = main_.get();
	this->addElement(std::move(main_));
	this->selfHandle = self;
}

UIOColoredBackground::UIOColoredBackground(Handle self, glm::vec4 c, UniqueReference<UIOBase, UIOBase> main_) {
	this->main = main_.get();
	this->addElement(std::move(main_));
	this->selfHandle = self;
	this->color = c;
}

ScreenRectangle UIOColoredBackground::updateSize(ScreenRectangle newScreenRectangle) {
	//this->screenRectangle = newScreenRectangle;
	this->screenRectangle = this->main->updateSize(newScreenRectangle);
	return this->screenRectangle;
	//return this->main->updateSize(newScreenRectangle);
}

int32_t UIOColoredBackground::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	depth = this->main->addRenderInfo(gameState, renderInfo, depth);
	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.getBottomLeft(), this->screenRectangle.getTopRight(), this->color, depth++);
	return depth;
}
