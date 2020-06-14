#include "common.h"

#include "UIOHotbar.h"
#include "UIOPad.h"
#include "UIOButton.h"
#include "UIOWindowTile.h"
#include "GameState.h"
#include "RenderInfo.h"

UIOHotbar::UIOHotbar(Handle self) {
	selfHandle = self;

	auto refMan = Locator<ReferenceManager<UIOBase>>::getService();

	auto tile = refMan->makeUniqueRef<UIOWindowTile>();

	for (int32_t i = 0; i < 10; i++) {
		auto e = refMan->makeUniqueRef<UIOPad>(refMan->makeUniqueRef<UIOButton>());
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

		slots.push_back(e.get());
		tile.get()->add(std::move(e));
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
		slots[selected]->screenRectangle.top + glm::vec2(0.01f, 0.01f),
		slots[selected]->screenRectangle.bot - glm::vec2(0.01f, 0.01f),
		{ 1,0,0,1 },
		depth
	);
	return depth + 1;
}
