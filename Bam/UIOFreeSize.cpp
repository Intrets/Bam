#include "common.h"

#include "UIOFreeSize.h"

UIOFreeSize::UIOFreeSize(Handle self) {
	this->selfHandle = self;
}

UIOFreeSize::UIOFreeSize(Handle self, UniqueReference<UIOBase, UIOBase> main) {
	this->selfHandle = self;
	this->addElement(std::move(main));
}

ScreenRectangle UIOFreeSize::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

	ScreenRectangle r = this->main.get()->getScreenRectangle();
	r.setPixelSize(newScreenRectangle.getPixelSize());
	this->main.get()->updateSize(r);

	return this->screenRectangle;
}

