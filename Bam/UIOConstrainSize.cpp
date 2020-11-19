#include "common.h"

#include "UIOConstrainSize.h"

UIOConstrainSize::UIOConstrainSize(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	this->selfHandle = self;
	this->addElement(std::move(main_));
}

ScreenRectangle UIOConstrainSize::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

	if (this->maybeWidth) {
		newScreenRectangle.setWidth(this->maybeWidth.value().getWidth(this->screenRectangle));
	}
	if (this->maybeHeight) {
		newScreenRectangle.setHeight(this->maybeHeight.value().getHeight(this->screenRectangle));
	}

	newScreenRectangle = this->main.get()->updateSize(newScreenRectangle);

	glm::vec2 p;
	switch (this->alignment) {
		case UIO::ALIGNMENT::TOP:
			p.x += this->screenRectangle.getWidth() / 2 - newScreenRectangle.getWidth() / 2;
			[[fallthrough]];
		case UIO::ALIGNMENT::TOPLEFT:
			p += this->screenRectangle.getTopLeft() - newScreenRectangle.getTopLeft();
			break;

		case UIO::ALIGNMENT::RIGHT:
			p.y -= this->screenRectangle.getHeight() / 2 - newScreenRectangle.getHeight() / 2;
			[[fallthrough]];
		case UIO::ALIGNMENT::TOPRIGHT:
			p += this->screenRectangle.getTopRight() - newScreenRectangle.getTopRight();
			break;

		case UIO::ALIGNMENT::BOTTOM:
			p.x -= this->screenRectangle.getWidth() / 2 - newScreenRectangle.getWidth() / 2;
			[[fallthrough]];
		case UIO::ALIGNMENT::BOTTOMRIGHT:
			p += this->screenRectangle.getBottomRight() - newScreenRectangle.getBottomRight();
			break;

		case UIO::ALIGNMENT::LEFT:
			p.y += this->screenRectangle.getHeight() / 2 - newScreenRectangle.getHeight() / 2;
			[[fallthrough]];
		case UIO::ALIGNMENT::BOTTOMLEFT:
			p += this->screenRectangle.getBottomLeft() - newScreenRectangle.getBottomLeft();
			break;

		case UIO::ALIGNMENT::CENTER:
			p.y -= this->screenRectangle.getHeight() / 2 - newScreenRectangle.getHeight() / 2;
			p.x += this->screenRectangle.getWidth() / 2 - newScreenRectangle.getWidth() / 2;
			break;

		default:
			break;
	}

	// Aligns to bottom left if the containing element does not fid in newScreenRectangle
	p = glm::max(this->screenRectangle.getBottomLeft(), newScreenRectangle.getBottomLeft() + p) - newScreenRectangle.getBottomLeft();

	this->screenRectangle = newScreenRectangle;
	this->translate(p);

	return this->screenRectangle;
}
