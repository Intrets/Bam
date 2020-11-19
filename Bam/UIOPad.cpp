#include "common.h"

#include "UIOPad.h"

UIOPad::UIOPad(Handle self) {
	this->selfHandle = self;
}

UIOPad::UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	this->selfHandle = self;
	this->addElement(std::move(main_));
}

UIOPad::UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_, UIOSizeType padding) {
	this->selfHandle = self;
	this->addElement(std::move(main_));

	this->bottom = padding;
	this->top = padding;
	this->right = padding;
	this->left = padding;
}

ScreenRectangle UIOPad::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

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

	main.get()->updateSize(newScreenRectangle);
	return this->screenRectangle;
}
