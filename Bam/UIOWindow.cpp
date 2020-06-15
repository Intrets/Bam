#include "common.h"

#include "UIOWindow.h"
#include "UIOConstrainSize.h"
#include "UIOPad.h"
#include "GameState.h"

UIOWindow::UIOWindow(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::getService();
	selfHandle = self;

	main = main_.get();

	auto mainPad = refMan->makeUniqueRef<UIOPad>(std::move(main_));
	mainPad.get()->top = { UIOSizeType::PX, 30 };
	mainPad.get()->right = { UIOSizeType::PX, 20 };
	mainPad.get()->bottom = { UIOSizeType::PX, 20 };

	addElement(std::move(mainPad));

	auto top = refMan->makeUniqueRef<UIOButton>();
	topBar = top.get();

	auto topConstrain = refMan->makeUniqueRef<UIOConstrainSize>(std::move(top));
	topConstrain.get()->maybeHeight = { UIOSizeType::PX, 30 };
	topConstrain.get()->alignment = CONSTRAIN_ALIGNMENT::TOP;

	addElement(std::move(topConstrain));

	auto setMoveOrigin = [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		this->mousePressedPosOffset = screenRectangle.getTopLeft() - gameState.getPlayerCursorScreenSpace();
		return false;
	};

	auto move = [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		if (topBar->down) {
			this->moveTopLeftTo(gameState.getPlayerCursorScreenSpace() + this->mousePressedPosOffset);
		}
		return false;
	};

	auto b = refMan->makeUniqueRef<UIOButton>();
	bottomBar = b.get();

	auto botConstrain = refMan->makeUniqueRef<UIOConstrainSize>(std::move(b));
	botConstrain.get()->maybeHeight = { UIOSizeType::PX, 20 };
	botConstrain.get()->alignment = CONSTRAIN_ALIGNMENT::BOTTOMRIGHT;

	addElement(std::move(botConstrain));

	auto r = refMan->makeUniqueRef<UIOButton>();
	rightBar = r.get();

	auto rightConstrain = refMan->makeUniqueRef<UIOConstrainSize>(std::move(r));
	rightConstrain.get()->maybeWidth = { UIOSizeType::PX, 20 };
	rightConstrain.get()->alignment = CONSTRAIN_ALIGNMENT::BOTTOMRIGHT;

	addElement(std::move(rightConstrain));


	auto scaleVertical = [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		if (bottomBar->down) {
			auto bottomRight = this->screenRectangle.getBottomRight();
			bottomRight.y = gameState.getPlayerCursorScreenSpace().y;
			if (this->screenRectangle.top.y - bottomRight.y < 0.2f) {
				bottomRight.y = this->screenRectangle.top.y - 0.2f;
			}
			this->screenRectangle.setBottomRight(bottomRight);
			this->updateSize(this->screenRectangle);
		}
		return false;
	};

	auto scaleHorizontal = [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		if (rightBar->down) {
			auto bottomRight = this->screenRectangle.getBottomRight();
			bottomRight.x = gameState.getPlayerCursorScreenSpace().x;
			if (bottomRight.x - this->screenRectangle.bot.x < 0.2f) {
				bottomRight.x = this->screenRectangle.bot.x + 0.2f;
			}
			this->screenRectangle.setBottomRight(bottomRight);
			this->updateSize(this->screenRectangle);
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

	for (auto& element : elements) {
		element.get()->updateSize(newScreenRectangle);
	}
	return screenRectangle;
}

int32_t UIOWindow::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	return UIOBase::addRenderInfo(gameState, renderInfo, depth + 1);
}
