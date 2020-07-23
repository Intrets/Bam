#include "common.h"

#include "UIOPad.h"

UIOPad::UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	this->selfHandle = self;
	this->main = main_.get();
	this->addElement(std::move(main_));
}

ScreenRectangle UIOPad::updateSize(ScreenRectangle newScreenRectangle) {
	glm::vec2 botScale;
	glm::vec2 topScale;

	if (this->right) {
		topScale.x = this->right.value().getWidth(newScreenRectangle);
	}
	if (this->top) {
		topScale.y = this->top.value().getHeight(newScreenRectangle);
	}
	if (this->left) {
		botScale.x = this->left.value().getWidth(newScreenRectangle);
	}
	if (this->bottom) {
		botScale.y = this->bottom.value().getHeight(newScreenRectangle);
	}

	newScreenRectangle.setBottomLeft(newScreenRectangle.getBottomLeft() + botScale);
	newScreenRectangle.setTopRight(newScreenRectangle.getTopRight() - topScale);

	this->screenRectangle = newScreenRectangle;
	main->updateSize(this->screenRectangle);
	return this->screenRectangle;
}
