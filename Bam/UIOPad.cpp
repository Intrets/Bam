#include "common.h"

#include "UIOPad.h"

UIOPad::UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	selfHandle = self;
	main = main_.get();
	addElement(std::move(main_));
}

ScreenRectangle UIOPad::updateSize(ScreenRectangle newScreenRectangle) {
	glm::vec2 botScale;
	glm::vec2 topScale;

	if (right) {
		topScale.x = right.value().getWidth(newScreenRectangle);
	}
	if (top) {
		topScale.y = top.value().getHeight(newScreenRectangle);
	}
	if (left) {
		botScale.x = left.value().getWidth(newScreenRectangle);
	}
	if (bottom) {
		botScale.y = bottom.value().getHeight(newScreenRectangle);
	}

	newScreenRectangle.setBottomLeft(newScreenRectangle.getBottomLeft() + botScale);
	newScreenRectangle.setTopRight(newScreenRectangle.getTopRight() - topScale);

	screenRectangle = newScreenRectangle;
	main->updateSize(screenRectangle);
	return screenRectangle;
}
