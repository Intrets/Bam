#include "common.h"

#include "UIOHotbar.h"
#include "UIOPad.h"
#include "UIOButton.h"
#include "UIOWindowTile.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "UIOShell.h"
#include "ControlState.h"
#include "UIOCallBackParams.h"

UIOHotbar::UIOHotbar(Handle self) {
	selfHandle = self;

	addGlobalBind({ CONTROLS::SCROLL_DOWN, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase*) -> CallBackBindResult {
		if (this->selected > 0) {
			selected += params.controlState.scrollDistance;
			selected = glm::clamp(selected, 0, 9);
		}
		return BIND_RESULT::CONTINUE;
	});

	addGlobalBind({ CONTROLS::SCROLL_UP, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase*) -> CallBackBindResult {
		if (this->selected < this->slots.size() - 1) {
			selected += params.controlState.scrollDistance;
			selected = glm::clamp(selected, 0, 9);
		}
		return BIND_RESULT::CONTINUE;
	});

	auto refMan = Locator<ReferenceManager<UIOBase>>::getService();

	auto tile = refMan->makeUniqueRef<UIOWindowTile>();

	for (int32_t i = 0; i < 10; i++) {
		auto but = refMan->makeUniqueRef<UIOButton>();
		but.get()->onPress = [&, i](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
			this->selected = i;
			return BIND_RESULT::CONTINUE;
		};

		slots.push_back(but.get());

		auto e = refMan->makeUniqueRef<UIOPad>(std::move(but));
		e.get()->bottom = { UIOSizeType::PX, 5 };
		e.get()->top = { UIOSizeType::PX, 5 };
		e.get()->left = { UIOSizeType::PX, 5 };
		e.get()->right = { UIOSizeType::PX, 5 };

		auto shell = refMan->makeUniqueRef<UIOShell>(std::move(e));

		slotSize.push_back(shell.get());

		tile.get()->add(std::move(shell));
	}

	slots[0]->addGlobalBind({ CONTROLS::TOOL_0, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 0;
		return BIND_RESULT::CONTINUE;
	});

	slots[1]->addGlobalBind({ CONTROLS::TOOL_1, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 1;
		return BIND_RESULT::CONTINUE;
	});

	slots[2]->addGlobalBind({ CONTROLS::TOOL_2, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 2;
		return BIND_RESULT::CONTINUE;
	});

	slots[3]->addGlobalBind({ CONTROLS::TOOL_3, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 3;
		return BIND_RESULT::CONTINUE;
	});

	slots[4]->addGlobalBind({ CONTROLS::TOOL_4, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 4;
		return BIND_RESULT::CONTINUE;
	});

	slots[5]->addGlobalBind({ CONTROLS::TOOL_5, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 5;
		return BIND_RESULT::CONTINUE;
	});

	slots[6]->addGlobalBind({ CONTROLS::TOOL_6, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 6;
		return BIND_RESULT::CONTINUE;
	});

	slots[7]->addGlobalBind({ CONTROLS::TOOL_7, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 7;
		return BIND_RESULT::CONTINUE;
	});

	slots[8]->addGlobalBind({ CONTROLS::TOOL_8, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 8;
		return BIND_RESULT::CONTINUE;
	});

	slots[9]->addGlobalBind({ CONTROLS::TOOL_9, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 9;
		return BIND_RESULT::CONTINUE;
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

	depth += 1;

	return depth;
}

CallBackBindResult UIOHotbar::runGlobalBinds(State& state) {
	auto& maybeSelected = this->tools[this->selected];
	CallBackBindResult res = UIOBase::runGlobalBinds(state);
	if (maybeSelected) {
		res |= maybeSelected.value().get()->runGlobalBinds(state);
	}
	return res;
}

CallBackBindResult UIOHotbar::runFocussedBinds(State& state) {
	auto& maybeSelected = this->tools[this->selected];
	CallBackBindResult res = UIOBase::runFocussedBinds(state);
	if (maybeSelected) {
		res |= maybeSelected.value().get()->runFocussedBinds(state);
	}
	return res;
}
