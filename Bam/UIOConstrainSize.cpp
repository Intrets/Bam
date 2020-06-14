#include "common.h"

#include "UIOConstrainSize.h"

UIOConstrainSize::UIOConstrainSize(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	selfHandle = self;
	main = main_.get();
	addElement(std::move(main_));
}

ScreenRectangle UIOConstrainSize::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	if (maybeWidth) {
		screenRectangle.setWidth(maybeWidth.value().getWidth(screenRectangle));
	}
	if (maybeHeight) {
		screenRectangle.setHeight(maybeHeight.value().getHeight(screenRectangle));
	}

	glm::vec2 p;
	switch (alignment) {
		case CONSTRAIN_ALIGNMENT::TOP:
			p.x += newScreenRectangle.getWidth() / 2 - screenRectangle.getWidth() / 2;
		case CONSTRAIN_ALIGNMENT::TOPLEFT:
			p += newScreenRectangle.getTopLeft() - screenRectangle.getTopLeft();
			break;

		case CONSTRAIN_ALIGNMENT::RIGHT:
			p.y -= newScreenRectangle.getHeight() / 2 - screenRectangle.getHeight() / 2;
		case CONSTRAIN_ALIGNMENT::TOPRIGHT:
			p += newScreenRectangle.getTopRight() - screenRectangle.getTopRight();
			break;

		case CONSTRAIN_ALIGNMENT::BOTTOM:
			p.x -= newScreenRectangle.getWidth() / 2 - screenRectangle.getWidth() / 2;
		case CONSTRAIN_ALIGNMENT::BOTTOMRIGHT:
			p += newScreenRectangle.getBottomRight() - screenRectangle.getBottomRight();
			break;

		case CONSTRAIN_ALIGNMENT::LEFT:
			p.y += newScreenRectangle.getHeight() / 2 - screenRectangle.getHeight() / 2;
		case CONSTRAIN_ALIGNMENT::BOTTOMLEFT:
			p += newScreenRectangle.getBottomLeft() - screenRectangle.getBottomLeft();
			break;

		default:
			break;
	}
	screenRectangle.translate(p);
	main->updateSize(screenRectangle);

	return screenRectangle;
}
