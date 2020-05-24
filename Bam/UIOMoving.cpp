#include "common.h"

#include "UIOMoving.h"
#include "UIOButton.h"
#include "GameState.h"

UIOMoving::UIOMoving(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	selfHandle = self;
	main = main_.get();
	addElement(std::move(main_));

	auto top = Locator<ReferenceManager<UIOBase>>::getService()->makeUniqueRef<UIOButton>();
	topBar = top.get();
	addElement(std::move(top));

	auto move = [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		if (topBar->down) {
			self_->moveTopLeftTo(gameState.getPlayerCursorScreenSpace());
		}
		return false;
	};

	addBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED }, move);
}

ScreenRectangle UIOMoving::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;

	glm::vec2 screenSize = screenRectangle.bottomRight() - screenRectangle.topLeft();
	//glm::vec2 top = screenRectangle.topLeft();

	ScreenRectangle b = screenRectangle;
	b.setBottomRight(screenRectangle.topRight() + glm::vec2(0, 0.05f * screenSize.y));

	topBar->updateSize(b);

	b = screenRectangle;
	b.setTopLeft(screenRectangle.topLeft() + glm::vec2(0, 0.05f * screenSize.y));

	main->updateSize(b);

	return screenRectangle;
}

int32_t UIOMoving::addRenderInfo(RenderInfo& renderInfo, int32_t depth) {
	return UIOBase::addRenderInfo(renderInfo, depth + 1);
}
