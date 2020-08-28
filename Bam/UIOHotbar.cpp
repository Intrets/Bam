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

	auto refMan = Locator<ReferenceManager<UIOBase>>::get();

	auto tile = refMan->makeUniqueRef<UIOGrid>(glm::ivec2(10, 1));

	for (int32_t i = 0; i < 10; i++) {
		auto but = refMan->makeUniqueRef<UIOButton>();
		but.get()->onPress = [this, i](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			this->activateTool(i, params);
			return BIND_RESULT::CONTINUE;
		};

		auto e = refMan->makeUniqueRef<UIOPad>(std::move(but));
		e.get()->bottom = { UIOSizeType::STATIC_PX, 2 };
		e.get()->top = { UIOSizeType::STATIC_PX, 2 };
		e.get()->left = { UIOSizeType::STATIC_PX, 2 };
		e.get()->right = { UIOSizeType::STATIC_PX, 2 };

		tile.get()->addElement(std::move(e));
	}

	for (auto [bind, i] : {
		std::make_tuple(ControlState::CONTROLS::TOOL_1, 0),
		std::make_tuple(ControlState::CONTROLS::TOOL_2, 1),
		std::make_tuple(ControlState::CONTROLS::TOOL_3, 2),
		std::make_tuple(ControlState::CONTROLS::TOOL_4, 3),
		std::make_tuple(ControlState::CONTROLS::TOOL_5, 4),
		std::make_tuple(ControlState::CONTROLS::TOOL_6, 5),
		std::make_tuple(ControlState::CONTROLS::TOOL_7, 6),
		std::make_tuple(ControlState::CONTROLS::TOOL_8, 7),
		std::make_tuple(ControlState::CONTROLS::TOOL_9, 8),
		std::make_tuple(ControlState::CONTROLS::TOOL_0, 9) }) {
		this->addGlobalBind({ bind }, [this, i = i](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			this->activateTool(i, params);
			return BIND_RESULT::CONTINUE;
		});
	}

	addElement(std::move(tile));
}

void UIOHotbar::activateTool(int32_t slot, UIOCallBackParams& params) {
	if (indexInArray(slot, this->tools)) {
		if (this->tools[slot].has_value()) {
			this->tools[slot].value()(params);
		}
	}
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
		this->getScreenRectangle().getTopRight(),
		this->getScreenRectangle().getBottomLeft(),
		{ 0.5f,0.5f,0.5f,1 },
		depth++
	);
	return depth;
}

void UIOHotbar::addTool(int32_t slot, std::function<void(UIOCallBackParams& params)> f) {
	if (indexInArray(slot, this->tools)) {
		this->tools[slot] = f;
	}
}
