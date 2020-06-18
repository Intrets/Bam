#include "common.h"

#include "UIOInvisible.h"

UIOInvisible::UIOInvisible(Handle self) {
	selfHandle = self;
}

ScreenRectangle UIOInvisible::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	return screenRectangle;
}
