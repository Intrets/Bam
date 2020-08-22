#include "common.h"

#include "UIOShell.h"

UIOShell::UIOShell(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	this->selfHandle = self;
	this->addElement(std::move(main_));
}

UIOShell::UIOShell(Handle self) {
	this->selfHandle = self;
}

ScreenRectangle UIOShell::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;
	for (auto& element : this->elements) {
		element.get()->updateSize(this->screenRectangle);
	}
	return this->screenRectangle;
}
