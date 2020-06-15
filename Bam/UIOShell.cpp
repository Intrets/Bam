#include "common.h"

#include "UIOShell.h"

UIOShell::UIOShell(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	selfHandle = self;
	main = main_.get();
	addElement(std::move(main_));
}

ScreenRectangle UIOShell::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	for (auto& element : elements) {
		element.get()->updateSize(newScreenRectangle);
	}
	return newScreenRectangle;
}
