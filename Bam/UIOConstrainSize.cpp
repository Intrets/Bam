#include "common.h"

#include "UIOConstrainSize.h"

UIOConstrainSize::UIOConstrainSize(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	this->selfHandle = self;
	this->main = main_.get();
	addElement(std::move(main_));
}

ScreenRectangle UIOConstrainSize::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;
	if (this->maybeWidth) {
		this->screenRectangle.setWidth(this->maybeWidth.value().getWidth(this->screenRectangle));
	}
	if (this->maybeHeight) {
		this->screenRectangle.setHeight(this->maybeHeight.value().getHeight(this->screenRectangle));
	}

	glm::vec2 p;
	switch (this->alignment) {
		case CONSTRAIN_ALIGNMENT::TOP:
			p.x += newScreenRectangle.getWidth() / 2 - this->screenRectangle.getWidth() / 2;
		case CONSTRAIN_ALIGNMENT::TOPLEFT:
			p += newScreenRectangle.getTopLeft() - this->screenRectangle.getTopLeft();
			break;

		case CONSTRAIN_ALIGNMENT::RIGHT:
			p.y -= newScreenRectangle.getHeight() / 2 - this->screenRectangle.getHeight() / 2;
		case CONSTRAIN_ALIGNMENT::TOPRIGHT:
			p += newScreenRectangle.getTopRight() - this->screenRectangle.getTopRight();
			break;

		case CONSTRAIN_ALIGNMENT::BOTTOM:
			p.x -= newScreenRectangle.getWidth() / 2 - this->screenRectangle.getWidth() / 2;
		case CONSTRAIN_ALIGNMENT::BOTTOMRIGHT:
			p += newScreenRectangle.getBottomRight() - this->screenRectangle.getBottomRight();
			break;

		case CONSTRAIN_ALIGNMENT::LEFT:
			p.y += newScreenRectangle.getHeight() / 2 - this->screenRectangle.getHeight() / 2;
		case CONSTRAIN_ALIGNMENT::BOTTOMLEFT:
			p += newScreenRectangle.getBottomLeft() - this->screenRectangle.getBottomLeft();
			break;

		default:
			break;
	}
	this->screenRectangle.translate(p);
	this->main->updateSize(this->screenRectangle);

	return this->screenRectangle;
}
