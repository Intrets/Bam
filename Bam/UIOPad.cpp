#include "common.h"

#include "UIOPad.h"

UIOPad::UIOPad(Handle self) {
	this->selfHandle = self;
}

ScreenRectangle UIOPad::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

	if (this->right.type == UIOPAD::TYPE::SIZETYPE) {
		newScreenRectangle.translateRight(-this->right.getSizeType().getWidth(newScreenRectangle));
	}
	if (this->top.type == UIOPAD::TYPE::SIZETYPE) {
		newScreenRectangle.translateTop(-this->top.getSizeType().getHeight(newScreenRectangle));
	}
	if (this->left.type == UIOPAD::TYPE::SIZETYPE) {
		newScreenRectangle.translateLeft(this->left.getSizeType().getWidth(newScreenRectangle));
	}
	if (this->bottom.type == UIOPAD::TYPE::SIZETYPE) {
		newScreenRectangle.translateBottom(this->bottom.getSizeType().getHeight(newScreenRectangle));
	}

	newScreenRectangle = main.get()->updateSize(newScreenRectangle);

	if (this->right.type == UIOPAD::TYPE::SHRINK) {
		this->screenRectangle.setRight(newScreenRectangle.getRight());
	}
	if (this->top.type == UIOPAD::TYPE::SHRINK) {
		this->screenRectangle.setTop(newScreenRectangle.getTop());
	}
	if (this->left.type == UIOPAD::TYPE::SHRINK) {
		this->screenRectangle.setLeft(newScreenRectangle.getLeft());
	}
	if (this->bottom.type == UIOPAD::TYPE::SHRINK) {
		this->screenRectangle.setBot(newScreenRectangle.getBot());
	}

	return this->screenRectangle;
}

UIO::TYPE UIOPad::getUIOType() {
	return UIO::TYPE::PAD;
}

UIOSizeType const& UIOPadType::getSizeType() const {
	assert(this->type == UIOPAD::TYPE::SIZETYPE);
	return this->size;
}

UIOPadType& UIOPadType::operator=(UIOPAD::TYPE const& t) {
	this->type = t;
	return *this;
}

UIOPadType& UIOPadType::operator=(UIOSizeType const& s) {
	this->type = UIOPAD::TYPE::SIZETYPE;
	this->size = s;
	return *this;
}

UIOPadType::UIOPadType() : UIOPadType(UIOPAD::TYPE::SHRINK) {
}

UIOPadType::UIOPadType(UIOPAD::TYPE t) : size(UIO::SIZETYPE::ABSOLUTE_HEIGHT, -1.0f), type(t) {
}

UIOPadType::UIOPadType(UIOSizeType s) : size(s), type(UIOPAD::TYPE::SIZETYPE) {

}
