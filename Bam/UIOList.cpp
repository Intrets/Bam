#include "common.h"

#include "UIOList.h"

UIOList::UIOList(Handle self, DIRECTION dir) {
	this->selfHandle = self;
	this->direction = dir;
}

ScreenRectangle UIOList::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

	ScreenRectangle rec = this->screenRectangle;
	for (auto& element : this->elements) {
		ScreenRectangle newRec = element.get()->updateSize(rec);

		if (this->direction == DIRECTION::HORIZONTAL) {
			rec.bot.x += newRec.getWidth();
		}
		else {
			rec.top.y -= newRec.getHeight();
		}
	}

	return this->screenRectangle;
}
