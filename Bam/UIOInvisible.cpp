#include "common.h"

#include "UIOInvisible.h"

UIOInvisible::UIOInvisible(Handle self) {
	this->selfHandle = self;
}

ScreenRectangle UIOInvisible::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;
	return this->screenRectangle;
}
