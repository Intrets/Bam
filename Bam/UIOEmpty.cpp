#include "common.h"

#include "UIOEmpty.h"

UIOEmpty::UIOEmpty(Handle self_) {
	this->selfHandle = self_;
}

ScreenRectangle UIOEmpty::updateSize(ScreenRectangle newScreenRectangle) {
	return newScreenRectangle;
}
