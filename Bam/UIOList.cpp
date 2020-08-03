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

		switch (this->direction) {
			case DIRECTION::LEFT:
				{
					glm::vec2 p = rec.getTopRight() - newRec.getTopRight();
					element.get()->translate(p);
					rec.top.x -= newRec.getWidth();
				}
				break;
			case DIRECTION::RIGHT:
				{
					glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
					element.get()->translate(p);
					rec.bot.x += newRec.getWidth();
				}
				break;
			case DIRECTION::UP:
				{
					glm::vec2 p = rec.getBottomLeft() - newRec.getBottomLeft();
					element.get()->translate(p);
					rec.bot.y += newRec.getHeight();
				}
				break;
			case DIRECTION::DOWN:
				{
					glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
					element.get()->translate(p);
					rec.top.y -= newRec.getHeight();
				}
				break;
			default:
				break;
		}
	}

	return this->screenRectangle;
}
