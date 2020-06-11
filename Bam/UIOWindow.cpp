#include "common.h"

#include "UIOWindow.h"
#include "GameState.h"

UIOWindow::UIOWindow(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	selfHandle = self;
	main = main_.get();
	addElement(std::move(main_));

	auto top = Locator<ReferenceManager<UIOBase>>::getService()->makeUniqueRef<UIOButton>();
	topBar = top.get();
	addElement(std::move(top));


	auto setMoveOrigin = [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		mousePressedPosOffset = screenRectangle.topLeft() - gameState.getPlayerCursorScreenSpace();
		return false;
	};

	auto move = [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		if (topBar->down) {
			this->moveTopLeftTo(gameState.getPlayerCursorScreenSpace() + this->mousePressedPosOffset);
		}
		return false;
	};

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
	addBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED }, move);
	addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, setMoveOrigin);
}

ScreenRectangle UIOWindow::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;

	glm::vec2 screenSize = screenRectangle.bottomRight() - screenRectangle.topLeft();
	glm::vec2 top = screenRectangle.topLeft();
	glm::vec2 bot = screenRectangle.bottomRight();

	{
		ScreenRectangle b = screenRectangle;
		b.set(top, { bot.x, top.y + 0.05f * screenSize.y });
		topBar->updateSize(b);
	}
	{
		ScreenRectangle b = screenRectangle;
		b.setTopLeft({ top.x + 0.95f * screenSize.x, top.y + 0.05f * screenSize.y });
		rightBar->updateSize(b);
	}
	{
		ScreenRectangle b = screenRectangle;
		b.setTopLeft({ top.x, top.y + 0.95f * screenSize.y });
		bottomBar->updateSize(b);
	}
	{
		ScreenRectangle b = screenRectangle;
		b.set({ top.x, top.y + 0.05f * screenSize.y }, top + 0.95f * screenSize);
		main->updateSize(b);
	}

	return screenRectangle;
}

int32_t UIOWindow::addRenderInfo(RenderInfo& renderInfo, int32_t depth) {
	return UIOBase::addRenderInfo(renderInfo, depth + 1);
}
