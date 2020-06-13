#include "common.h"

#include "UIOConstrainSize.h"

UIOConstrainSize::UIOConstrainSize(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	selfHandle = self;
	main = main_.get();
	addElement(std::move(main_));
}

ScreenRectangle UIOConstrainSize::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	if (width) {
		float widthS = static_cast<float>(width.value()) / screenRectangle.getPixelSize().x;
		screenRectangle.setWidth(widthS);
	}
	if (height) {
		float heightS = static_cast<float>(height.value()) / screenRectangle.getPixelSize().y;
		screenRectangle.setHeight(heightS);
	}

	glm::vec2 p;
	switch (alignment) {
		case CONSTRAIN_ALIGNMENT::TOP:
			break;
		case CONSTRAIN_ALIGNMENT::BOTTOM:
			break;
		case CONSTRAIN_ALIGNMENT::LEFT:
			break;
		case CONSTRAIN_ALIGNMENT::RIGHT:
			break;
		case CONSTRAIN_ALIGNMENT::TOPLEFT:
			p = newScreenRectangle.getTopLeft() - screenRectangle.getTopLeft();
			break;
		case CONSTRAIN_ALIGNMENT::TOPRIGHT:
			p = newScreenRectangle.getTopRight() - screenRectangle.getTopRight();
			break;
		case CONSTRAIN_ALIGNMENT::BOTTOMLEFT:
			p = newScreenRectangle.getBottomLeft() - screenRectangle.getBottomLeft();
			break;
		case CONSTRAIN_ALIGNMENT::BOTTOMRIGHT:
			p = newScreenRectangle.getBottomRight() - screenRectangle.getBottomRight();
			break;
		default:
			break;
	}
	screenRectangle.translate(p);
	//newScreenRectangle.translate(p);
	main->updateSize(screenRectangle);

	return screenRectangle;
}
