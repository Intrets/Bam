#include "common.h"

#include "UIOHotbar.h"
#include "UIOPad.h"
#include "UIOButton.h"
#include "UIOWindowTile.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "UIOShell.h"

UIOHotbar::UIOHotbar(Handle self) {
	selfHandle = self;

	auto refMan = Locator<ReferenceManager<UIOBase>>::getService();

	auto tile = refMan->makeUniqueRef<UIOWindowTile>();

	for (int32_t i = 0; i < 10; i++) {
		auto but = refMan->makeUniqueRef<UIOButton>();

		slots.push_back(but.get());

		auto e = refMan->makeUniqueRef<UIOPad>(std::move(but));
		e.get()->bottom = { UIOSizeType::PX, 5 };
		e.get()->top = { UIOSizeType::PX, 5 };
		e.get()->left = { UIOSizeType::PX, 5 };
		e.get()->right = { UIOSizeType::PX, 5 };

		auto select = [&, i](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
			if (self_->screenRectangle.contains(gameState.getPlayerCursorScreenSpace())) {
				this->selected = i;
				return true;
			}
			return false;
		};

		e.get()->main->addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, select);

		auto shell = refMan->makeUniqueRef<UIOShell>(std::move(e));

		slotSize.push_back(shell.get());

		tile.get()->add(std::move(shell));
	}

	addElement(std::move(tile));
}

ScreenRectangle UIOHotbar::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	for (auto& element : elements) {
		element.get()->updateSize(newScreenRectangle);
	}
	return screenRectangle;
}

int32_t UIOHotbar::addRenderInfo(RenderInfo& renderInfo, int32_t depth) {
	depth = UIOBase::addRenderInfo(renderInfo, depth);
	renderInfo.uiRenderInfo.addRectangle(
		screenRectangle.top,
		screenRectangle.bot,
		{ 0.5f,0.5f,0.5f,1 },
		depth + 1
	);
	renderInfo.uiRenderInfo.addRectangle(
		slotSize[selected]->screenRectangle.top,
		slotSize[selected]->screenRectangle.bot,
		{ 1,0,0,1 },
		depth
	);
	return depth + 2;
}
