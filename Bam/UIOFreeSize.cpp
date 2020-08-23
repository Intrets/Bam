#include "common.h"

#include "UIOFreeSize.h"

UIOFreeSize::UIOFreeSize(Handle self, UniqueReference<UIOBase, UIOBase> main) {
	this->selfHandle = self;
	this->addElement(std::move(main));
}

ScreenRectangle UIOFreeSize::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;
	for (auto& element : this->elements) {
		ScreenRectangle r = element.get()->getScreenRectangle();
		r.setPixelSize(newScreenRectangle.getPixelSize());
		element.get()->updateSize(r);
	}
	return this->screenRectangle;
}

