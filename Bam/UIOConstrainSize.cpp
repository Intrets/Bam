#include "common.h"

#include "UIOConstrainSize.h"

UIOConstrainSize::UIOConstrainSize(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	this->selfHandle = self;
	this->main = main_.get();
	addElement(std::move(main_));
}

ScreenRectangle UIOConstrainSize::updateSize(ScreenRectangle newScreenRectangle) {
	//this->screenRectangle = newScreenRectangle;
	//if (this->maybeWidth) {
	//	this->screenRectangle.setWidth(this->maybeWidth.value().getWidth(this->screenRectangle));
	//}
	//if (this->maybeHeight) {
	//	this->screenRectangle.setHeight(this->maybeHeight.value().getHeight(this->screenRectangle));
	//}

	//glm::vec2 p;
	//switch (this->alignment) {
	//	case CONSTRAIN_ALIGNMENT::TOP:
	//		p.x += newScreenRectangle.getWidth() / 2 - this->screenRectangle.getWidth() / 2;
	//	case CONSTRAIN_ALIGNMENT::TOPLEFT:
	//		p += newScreenRectangle.getTopLeft() - this->screenRectangle.getTopLeft();
	//		break;

	//	case CONSTRAIN_ALIGNMENT::RIGHT:
	//		p.y -= newScreenRectangle.getHeight() / 2 - this->screenRectangle.getHeight() / 2;
	//	case CONSTRAIN_ALIGNMENT::TOPRIGHT:
	//		p += newScreenRectangle.getTopRight() - this->screenRectangle.getTopRight();
	//		break;

	//	case CONSTRAIN_ALIGNMENT::BOTTOM:
	//		p.x -= newScreenRectangle.getWidth() / 2 - this->screenRectangle.getWidth() / 2;
	//	case CONSTRAIN_ALIGNMENT::BOTTOMRIGHT:
	//		p += newScreenRectangle.getBottomRight() - this->screenRectangle.getBottomRight();
	//		break;

	//	case CONSTRAIN_ALIGNMENT::LEFT:
	//		p.y += newScreenRectangle.getHeight() / 2 - this->screenRectangle.getHeight() / 2;
	//	case CONSTRAIN_ALIGNMENT::BOTTOMLEFT:
	//		p += newScreenRectangle.getBottomLeft() - this->screenRectangle.getBottomLeft();
	//		break;

	//	case CONSTRAIN_ALIGNMENT::CENTER:


	//	default:
	//		break;
	//}
	//this->screenRectangle.translate(p);
	//this->main->updateSize(this->screenRectangle);

	this->screenRectangle = newScreenRectangle;

	if (this->maybeWidth) {
		newScreenRectangle.setWidth(this->maybeWidth.value().getWidth(newScreenRectangle));
	}
	if (this->maybeHeight) {
		newScreenRectangle.setHeight(this->maybeHeight.value().getHeight(newScreenRectangle));
	}

	newScreenRectangle = this->main->updateSize(newScreenRectangle);

	glm::vec2 p;
	switch (this->alignment) {
		case CONSTRAIN_ALIGNMENT::TOP:
			p.x += this->screenRectangle.getWidth() / 2 - newScreenRectangle.getWidth() / 2;
		case CONSTRAIN_ALIGNMENT::TOPLEFT:
			p += this->screenRectangle.getTopLeft() - newScreenRectangle.getTopLeft();
			break;

		case CONSTRAIN_ALIGNMENT::RIGHT:
			p.y -= this->screenRectangle.getHeight() / 2 - newScreenRectangle.getHeight() / 2;
		case CONSTRAIN_ALIGNMENT::TOPRIGHT:
			p += this->screenRectangle.getTopRight() - newScreenRectangle.getTopRight();
			break;

		case CONSTRAIN_ALIGNMENT::BOTTOM:
			p.x -= this->screenRectangle.getWidth() / 2 - newScreenRectangle.getWidth() / 2;
		case CONSTRAIN_ALIGNMENT::BOTTOMRIGHT:
			p += this->screenRectangle.getBottomRight() - newScreenRectangle.getBottomRight();
			break;

		case CONSTRAIN_ALIGNMENT::LEFT:
			p.y += this->screenRectangle.getHeight() / 2 - newScreenRectangle.getHeight() / 2;
		case CONSTRAIN_ALIGNMENT::BOTTOMLEFT:
			p += this->screenRectangle.getBottomLeft() - newScreenRectangle.getBottomLeft();
			break;
		default:
			break;
	}

	this->main->translate(p);

	return this->screenRectangle;
}
