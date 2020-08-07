#include "common.h"

#include "UIOHotbar.h"
#include "UIOPad.h"
#include "UIOButton.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "UIOShell.h"
#include "ControlState.h"
#include "UIOCallBackParams.h"
#include "UIOGrid.h"

UIOHotbar::UIOHotbar(Handle self) {
	this->selfHandle = self;

	addGlobalBind({ ControlState::CONTROLS::SCROLL_DOWN, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase*) -> CallBackBindResult {
		if (this->selected > 0) {
			this->selected += params.controlState.scrollDistance;
			this->selected = glm::clamp(this->selected, 0, 9);
		}
		return BIND_RESULT::CONTINUE;
	});

	addGlobalBind({ ControlState::CONTROLS::SCROLL_UP, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& params, UIOBase*) -> CallBackBindResult {
		if (this->selected < this->slots.size() - 1) {
			this->selected += params.controlState.scrollDistance;
			this->selected = glm::clamp(this->selected, 0, 9);
		}
		return BIND_RESULT::CONTINUE;
	});

	auto refMan = Locator<ReferenceManager<UIOBase>>::get();

	auto tile = refMan->makeUniqueRef<UIOGrid>(glm::ivec2(10, 1));

	for (int32_t i = 0; i < 10; i++) {
		auto but = refMan->makeUniqueRef<UIOButton>();
		but.get()->onPress = [&, i](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
			this->selected = i;
			return BIND_RESULT::CONTINUE;
		};

		this->slots.push_back(but.get());

		auto e = refMan->makeUniqueRef<UIOPad>(std::move(but));
		e.get()->bottom = { UIOSizeType::PX, 2 };
		e.get()->top = { UIOSizeType::PX, 2 };
		e.get()->left = { UIOSizeType::PX, 2 };
		e.get()->right = { UIOSizeType::PX, 2 };

		auto shell = refMan->makeUniqueRef<UIOShell>(std::move(e));

		this->slotSize.push_back(shell.get());

		tile.get()->addElement(std::move(shell));
	}

	this->slots[0]->addGlobalBind({ ControlState::CONTROLS::TOOL_0, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 0;
		return BIND_RESULT::CONTINUE;
	});

	this->slots[1]->addGlobalBind({ ControlState::CONTROLS::TOOL_1, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 1;
		return BIND_RESULT::CONTINUE;
	});

	this->slots[2]->addGlobalBind({ ControlState::CONTROLS::TOOL_2, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 2;
		return BIND_RESULT::CONTINUE;
	});

	this->slots[3]->addGlobalBind({ ControlState::CONTROLS::TOOL_3, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 3;
		return BIND_RESULT::CONTINUE;
	});

	this->slots[4]->addGlobalBind({ ControlState::CONTROLS::TOOL_4, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 4;
		return BIND_RESULT::CONTINUE;
	});

	this->slots[5]->addGlobalBind({ ControlState::CONTROLS::TOOL_5, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 5;
		return BIND_RESULT::CONTINUE;
	});

	this->slots[6]->addGlobalBind({ ControlState::CONTROLS::TOOL_6, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 6;
		return BIND_RESULT::CONTINUE;
	});

	this->slots[7]->addGlobalBind({ ControlState::CONTROLS::TOOL_7, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 7;
		return BIND_RESULT::CONTINUE;
	});

	this->slots[8]->addGlobalBind({ ControlState::CONTROLS::TOOL_8, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 8;
		return BIND_RESULT::CONTINUE;
	});

	this->slots[9]->addGlobalBind({ ControlState::CONTROLS::TOOL_9, ControlState::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams&, UIOBase*) -> CallBackBindResult {
		this->selected = 9;
		return BIND_RESULT::CONTINUE;
	});

	addElement(std::move(tile));
}

ScreenRectangle UIOHotbar::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;
	for (auto& element : this->elements) {
		element.get()->updateSize(newScreenRectangle);
	}
	return this->screenRectangle;
}

int32_t UIOHotbar::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	depth = this->UIOBase::addRenderInfo(gameState, renderInfo, depth);
	renderInfo.uiRenderInfo.addRectangle(
		this->slotSize[this->selected]->screenRectangle.top,
		this->slotSize[this->selected]->screenRectangle.bot,
		{ 1,0,0,1 },
		depth++
	);
	renderInfo.uiRenderInfo.addRectangle(
		this->screenRectangle.top,
		this->screenRectangle.bot,
		{ 0.5f,0.5f,0.5f,1 },
		depth++
	);
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
