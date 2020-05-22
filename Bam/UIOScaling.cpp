#include "common.h"

#include "UIOScaling.h"
#include "UIOBasicWindow.h"
#include "RenderInfo.h"

UIOScaling::UIOScaling(Handle self, UniqueReference<UIOBase, UIOBase> center_) {
	selfHandle = self;
	center = center_.get();
	addElement(std::move(center_));

	auto b = Locator<ReferenceManager<UIOBase>>::getService()->makeUniqueRef<UIOBasicWindow>();
	bottomBar = b.get();
	addElement(std::move(b));

	auto r = Locator<ReferenceManager<UIOBase>>::getService()->makeUniqueRef<UIOBasicWindow>();
	rightBar = r.get();
	addElement(std::move(r));

	auto onClick = [&](GameState& gameState, UIOBase* self) -> bool {
		this->scale(glm::vec2(0.9f));
		std::cout << "resize\n";
		return false;
	};

	auto onClickR = [&](GameState& gameState, UIOBase* self) -> bool {
		this->scale(glm::vec2(1/0.9f));
		std::cout << "resize\n";
		return false;
	};

	rightBar->addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_DOWN }, onClick);
	rightBar->addBind({ CONTROLS::ACTION2, CONTROLSTATE::CONTROLSTATE_DOWN }, onClickR);

}

ScreenRectangle UIOScaling::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;

	glm::vec2 screenSize = screenRectangle.bottomRight() - screenRectangle.topLeft();
	glm::vec2 top = screenRectangle.topLeft();

	ScreenRectangle b = screenRectangle;
	b.setTopLeft(top + glm::vec2(0, 0.9f*screenSize.y));

	bottomBar->updateSize(b);
	
	ScreenRectangle r = screenRectangle;
	r.setTopLeft(top + glm::vec2(0.9f * screenSize.x, 0));

	rightBar->updateSize(r);

	ScreenRectangle c = screenRectangle;
	c.setBottomRight(top + 0.9f * screenSize);
	center->updateSize(c);

	return screenRectangle;
}

int32_t UIOScaling::addRenderInfo(RenderInfo& renderInfo, int32_t depth) {
	return UIOBase::addRenderInfo(renderInfo, depth + 1);
}
