#include "common.h"

#include "UIOList.h"

UIOList::UIOList(Handle self, UIO::DIR dir) {
	this->selfHandle = self;
	this->direction = dir;
}

ScreenRectangle UIOList::updateSize(ScreenRectangle newScreenRectangle) {
	ScreenRectangle rec = newScreenRectangle;

	switch (this->direction) {
		case UIO::DIR::LEFT:
			for (auto& element : this->elements) {
				ScreenRectangle newRec = element.get()->updateSize(rec);
				glm::vec2 p = rec.getTopRight() - newRec.getTopRight();
				element.get()->translate(p);
				rec.translateRight(-newRec.getWidth());
			}
			newScreenRectangle.setTopLeft(rec.getTopRight());
			break;
		case UIO::DIR::RIGHT:
			for (auto& element : this->elements) {
				ScreenRectangle newRec = element.get()->updateSize(rec);
				glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
				element.get()->translate(p);
				rec.translateLeft(newRec.getWidth());
			}
			newScreenRectangle.setTopRight(rec.getTopLeft());
			break;
		case UIO::DIR::UP:
			for (auto& element : this->elements) {
				ScreenRectangle newRec = element.get()->updateSize(rec);
				glm::vec2 p = rec.getBottomLeft() - newRec.getBottomLeft();
				element.get()->translate(p);
				rec.translateBottom(newRec.getHeight());
			}
			newScreenRectangle.setTopRight(rec.getBottomRight());
			break;
		case UIO::DIR::DOWN:
			for (auto& element : this->elements) {
				ScreenRectangle newRec = element.get()->updateSize(rec);
				glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
				element.get()->translate(p);
				rec.translateTop(-newRec.getHeight());
			}
			newScreenRectangle.setBottomLeft(rec.getTopLeft());
			break;

		case UIO::DIR::LEFT_REVERSE:
			for (auto it = this->elements.rbegin(); it != this->elements.rend(); ++it) {
				auto& element = *it;
				ScreenRectangle newRec = element.get()->updateSize(rec);
				glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
				element.get()->translate(p);
				rec.translateLeft(newRec.getWidth());
			}
			newScreenRectangle.setTopRight(rec.getTopLeft());
			break;
		case UIO::DIR::RIGHT_REVERSE:
			for (auto it = this->elements.rbegin(); it != this->elements.rend(); ++it) {
				auto& element = *it;
				ScreenRectangle newRec = element.get()->updateSize(rec);
				glm::vec2 p = rec.getTopRight() - newRec.getTopRight();
				element.get()->translate(p);
				rec.translateRight(-newRec.getWidth());
			}
			newScreenRectangle.setTopLeft(rec.getTopRight());
			break;
		case UIO::DIR::UP_REVERSE:
			for (auto it = this->elements.rbegin(); it != this->elements.rend(); ++it) {
				auto& element = *it;
				ScreenRectangle newRec = element.get()->updateSize(rec);
				glm::vec2 p = rec.getTopLeft() - newRec.getTopLeft();
				element.get()->translate(p);
				rec.translateTop(-newRec.getHeight());
			}
			newScreenRectangle.setBottomLeft(rec.getTopLeft());
			break;
		case UIO::DIR::DOWN_REVERSE:
			for (auto it = this->elements.rbegin(); it != this->elements.rend(); ++it) {
				auto& element = *it;
				ScreenRectangle newRec = element.get()->updateSize(rec);
				glm::vec2 p = rec.getBottomLeft() - newRec.getBottomLeft();
				element.get()->translate(p);
				rec.translateBottom(newRec.getHeight());
			}
			newScreenRectangle.setTopRight(rec.getBottomRight());
			break;
		default:
			break;
	}

	this->screenRectangle = newScreenRectangle;

	return this->screenRectangle;
}

UIO::TYPE UIOList::getUIOType() {
	return UIO::TYPE::LIST;
}
