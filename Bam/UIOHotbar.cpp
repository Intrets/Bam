#include "common.h"

#include "UIOHotbar.h"
#include "UIOPad.h"
#include "UIOButton.h"
#include "UIOWindowTile.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "UIOShell.h"
#include "ControlState.h"

UIOHotbar::UIOHotbar(Handle self) {
	selfHandle = self;

	addBind({ CONTROLS::SCROLL_DOWN, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState& controlState, UIOBase*) -> bool {
		if (this->selected > 0) {
			selected += controlState.scrollDistance;
			selected = glm::clamp(selected, 0, 9);
		}
		return true;
	});

	addBind({ CONTROLS::SCROLL_UP, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState& controlState, UIOBase*) -> bool {
		if (this->selected < this->slots.size() - 1) {
			selected += controlState.scrollDistance;
			selected = glm::clamp(selected, 0, 9);
		}
		return true;
	});

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

	slots[0]->addBind({ CONTROLS::TOOL_0, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState&, UIOBase*) -> bool {
		this->selected = 0;
		return true;
	});

	slots[1]->addBind({ CONTROLS::TOOL_1, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState&, UIOBase*) -> bool {
		this->selected = 1;
		return true;
	});

	slots[2]->addBind({ CONTROLS::TOOL_2, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState&, UIOBase*) -> bool {
		this->selected = 2;
		return true;
	});

	slots[3]->addBind({ CONTROLS::TOOL_3, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState&, UIOBase*) -> bool {
		this->selected = 3;
		return true;
	});

	slots[4]->addBind({ CONTROLS::TOOL_4, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState&, UIOBase*) -> bool {
		this->selected = 4;
		return true;
	});

	slots[5]->addBind({ CONTROLS::TOOL_5, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState&, UIOBase*) -> bool {
		this->selected = 5;
		return true;
	});

	slots[6]->addBind({ CONTROLS::TOOL_6, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState&, UIOBase*) -> bool {
		this->selected = 6;
		return true;
	});

	slots[7]->addBind({ CONTROLS::TOOL_7, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState&, UIOBase*) -> bool {
		this->selected = 7;
		return true;
	});

	slots[8]->addBind({ CONTROLS::TOOL_8, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState&, UIOBase*) -> bool {
		this->selected = 8;
		return true;
	});

	slots[9]->addBind({ CONTROLS::TOOL_9, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState&, ControlState&, UIOBase*) -> bool {
		this->selected = 9;
		return true;
	});

	addElement(std::move(tile));
}

ScreenRectangle UIOHotbar::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	for (auto& element : elements) {
		element.get()->updateSize(newScreenRectangle);
	}
	return screenRectangle;
}

int32_t UIOHotbar::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	depth = UIOBase::addRenderInfo(gameState, renderInfo, depth);
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
