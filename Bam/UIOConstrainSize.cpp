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
		newScreenRectangle.setWidth(this->maybeWidth.value().getWidth(this->screenRectangle));
	}
	if (this->maybeHeight) {
		newScreenRectangle.setHeight(this->maybeHeight.value().getHeight(this->screenRectangle));
	}

	newScreenRectangle = this->main->updateSize(newScreenRectangle);

	glm::vec2 p;
	switch (this->alignment) {
		case UIOConstrainSize::ALIGNMENT::TOP:
			p.x += this->screenRectangle.getWidth() / 2 - newScreenRectangle.getWidth() / 2;
			[[fallthrough]];
		case UIOConstrainSize::ALIGNMENT::TOPLEFT:
			p += this->screenRectangle.getTopLeft() - newScreenRectangle.getTopLeft();
			break;

		case UIOConstrainSize::ALIGNMENT::RIGHT:
			p.y -= this->screenRectangle.getHeight() / 2 - newScreenRectangle.getHeight() / 2;
			[[fallthrough]];
		case UIOConstrainSize::ALIGNMENT::TOPRIGHT:
			p += this->screenRectangle.getTopRight() - newScreenRectangle.getTopRight();
			break;

		case UIOConstrainSize::ALIGNMENT::BOTTOM:
			p.x -= this->screenRectangle.getWidth() / 2 - newScreenRectangle.getWidth() / 2;
			[[fallthrough]];
		case UIOConstrainSize::ALIGNMENT::BOTTOMRIGHT:
			p += this->screenRectangle.getBottomRight() - newScreenRectangle.getBottomRight();
			break;

		case UIOConstrainSize::ALIGNMENT::LEFT:
			p.y += this->screenRectangle.getHeight() / 2 - newScreenRectangle.getHeight() / 2;
			[[fallthrough]];
		case UIOConstrainSize::ALIGNMENT::BOTTOMLEFT:
			p += this->screenRectangle.getBottomLeft() - newScreenRectangle.getBottomLeft();
			break;

		case UIOConstrainSize::ALIGNMENT::CENTER:
			p.y -= this->screenRectangle.getHeight() / 2 - newScreenRectangle.getHeight() / 2;
			p.x += this->screenRectangle.getWidth() / 2 - newScreenRectangle.getWidth() / 2;
	p = glm::max(glm::vec2(0.0f, 0.0f), p);
			break;

		default:
			break;
	}


	this->screenRectangle = newScreenRectangle;
	this->translate(p);

	return this->screenRectangle;
}
