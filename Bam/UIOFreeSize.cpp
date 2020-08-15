#include "common.h"

#include "UIOFreeSize.h"

UIOFreeSize::UIOFreeSize(Handle self, UniqueReference<UIOBase, UIOBase> main) {
	this->selfHandle = self;
	this->addElement(std::move(main));
}

ScreenRectangle UIOFreeSize::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;
	for (auto& element : this->elements) {
		auto r = element.get()->screenRectangle;
		r.screenPixels = newScreenRectangle.screenPixels;
		element.get()->updateSize(r);
	}
	return this->screenRectangle;
}

