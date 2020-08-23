#include "common.h"

#include "UIOList.h"

UIOList::UIOList(Handle self, DIR dir) {
	this->selfHandle = self;
	this->direction = dir;
}

ScreenRectangle UIOList::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

	ScreenRectangle rec = this->screenRectangle;

	for (auto& element : this->elements) {
		ScreenRectangle newRec = element.get()->updateSize(rec);

		switch (this->direction) {
			case DIR::LEFT:
				{
					glm::vec2 p = rec.getTopRight() - newRec.getTopRight();
					element.get()->translate(p);
					rec.translateRight(-newRec.getWidth());
					//rec.top.x -= newRec.getWidth();
				}
				break;
			case DIR::RIGHT:
				{
					glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
					element.get()->translate(p);
					rec.translateLeft(newRec.getWidth());
					//rec.bot.x += newRec.getWidth();
				}
				break;
			case DIR::UP:
				{
					glm::vec2 p = rec.getBottomLeft() - newRec.getBottomLeft();
					element.get()->translate(p);
					rec.translateBottom(newRec.getHeight());
					//rec.bot.y += newRec.getHeight();
				}
				break;
			case DIR::DOWN:
				{
					glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
					element.get()->translate(p);
					rec.translateTop(-newRec.getHeight());
					//rec.top.y -= newRec.getHeight();
				}
				break;
			default:
				break;
		}
	}

	return this->screenRectangle;
}
