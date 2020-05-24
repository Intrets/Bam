#include "common.h"

#include "UIOScaling.h"
#include "UIOBasicWindow.h"
#include "RenderInfo.h"
#include "UIOButton.h"
#include "GameState.h"

UIOScaling::UIOScaling(Handle self, UniqueReference<UIOBase, UIOBase> center_) {
	selfHandle = self;
	center = center_.get();
	addElement(std::move(center_));

	auto b = Locator<ReferenceManager<UIOBase>>::getService()->makeUniqueRef<UIOButton>();
	bottomBar = b.get();
	addElement(std::move(b));

	auto r = Locator<ReferenceManager<UIOBase>>::getService()->makeUniqueRef<UIOButton>();
	rightBar = r.get();
	addElement(std::move(r));

	auto scaleVertical = [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		if (bottomBar->down) {
			auto bottomRight = self_->screenRectangle.bottomRight();
			bottomRight.y = gameState.getPlayerCursorScreenSpace().y;
			self_->screenRectangle.setBottomRight(bottomRight);
			self_->updateSize(self_->screenRectangle);
		}
		return false;
	};

	auto scaleHorizontal = [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		if (rightBar->down) {
			auto bottomRight = self_->screenRectangle.bottomRight();
			bottomRight.x = gameState.getPlayerCursorScreenSpace().x;
			self_->screenRectangle.setBottomRight(bottomRight);
			self_->updateSize(self_->screenRectangle);
		}
		return false;
	};

	addBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED }, scaleVertical);
	addBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED }, scaleHorizontal);

}

ScreenRectangle UIOScaling::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;

	glm::vec2 screenSize = screenRectangle.bottomRight() - screenRectangle.topLeft();
	glm::vec2 top = screenRectangle.topLeft();

	ScreenRectangle b = screenRectangle;
	b.setTopLeft(top + glm::vec2(0, 0.95f * screenSize.y));

	bottomBar->updateSize(b);

	ScreenRectangle r = screenRectangle;
	r.setTopLeft(top + glm::vec2(0.95f * screenSize.x, 0));

	rightBar->updateSize(r);

	ScreenRectangle c = screenRectangle;
	c.setBottomRight(top + 0.95f * screenSize);
	center->updateSize(c);

	return screenRectangle;
}

int32_t UIOScaling::addRenderInfo(RenderInfo& renderInfo, int32_t depth) {
	return UIOBase::addRenderInfo(renderInfo, depth + 1);
}
