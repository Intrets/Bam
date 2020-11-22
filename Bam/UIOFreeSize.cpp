#include "common.h"

#include "UIOFreeSize.h"

UIOFreeSize::UIOFreeSize(Handle self) {
	this->selfHandle = self;
}

ScreenRectangle UIOFreeSize::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

	ScreenRectangle r = this->main.get()->getScreenRectangle();

	if (r.getPixelSize().x != 0 && r.getPixelSize().y != 0) {
		glm::vec2 ratio = glm::vec2(r.getPixelSize()) / glm::vec2(newScreenRectangle.getPixelSize());
		glm::vec2 size = r.getAbsSize() * ratio;
		r.setSize(size);
	}

	r.setPixelSize(newScreenRectangle.getPixelSize());
	this->main.get()->updateSize(r);

	return this->screenRectangle;
}

