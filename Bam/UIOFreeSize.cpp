#include "common.h"

#include "UIOFreeSize.h"

UIOFreeSize::UIOFreeSize(Handle self, UniqueReference<UIOBase, UIOBase> main) {
	selfHandle = self;
	addElement(std::move(main));
}

ScreenRectangle UIOFreeSize::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle.screenPixels = newScreenRectangle.screenPixels;
	for (auto& element : elements) {
		auto r = element.get()->screenRectangle;
		r.screenPixels = newScreenRectangle.screenPixels;
		element.get()->updateSize(r);
	}
	return screenRectangle;
}
