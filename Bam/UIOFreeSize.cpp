#include "common.h"

#include "UIOFreeSize.h"

UIOFreeSize::UIOFreeSize(Handle self) {
	this->selfHandle = self;
}

ScreenRectangle UIOFreeSize::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

	ScreenRectangle r = this->main.get()->getScreenRectangle();
	r.setPixelSize(newScreenRectangle.getPixelSize());
	this->main.get()->updateSize(r);

	return this->screenRectangle;
}

