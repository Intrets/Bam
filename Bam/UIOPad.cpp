#include "common.h"

#include "UIOPad.h"

UIOPad::UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	selfHandle = self;
	main = main_.get();
	addElement(std::move(main_));
}

ScreenRectangle UIOPad::updateSize(ScreenRectangle newScreenRectangle) {
	auto b = glm::vec2(bot) / glm::vec2(newScreenRectangle.getPixelSize());
	newScreenRectangle.setBottomLeft(newScreenRectangle.getBottomLeft() + b);

	auto t = glm::vec2(top) / glm::vec2(newScreenRectangle.getPixelSize());
	newScreenRectangle.setTopRight(newScreenRectangle.getTopRight() - t);
	screenRectangle = newScreenRectangle;
	main->updateSize(screenRectangle);
	return screenRectangle;
}
