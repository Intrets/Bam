#include "common.h"

#include "UIOButton.h"
#include "GameState.h"
#include "RenderInfo.h"

UIOButton::UIOButton(Handle self) {
	selfHandle = self;

	addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> CallBackBindResult {
		if (this->contains(gameState.getPlayerCursorScreenSpace())) {
			this->down = true;
			return BIND_RESULT::FOCUS | BIND_RESULT::CONSUME;
		}
		return BIND_RESULT::CONTINUE;
	});

	addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_RELEASED }, [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> CallBackBindResult {
		this->down = false;
		return BIND_RESULT::CONTINUE;
	});
}

ScreenRectangle UIOButton::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	return screenRectangle;
}

int32_t UIOButton::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	glm::vec4 color;
	if (down) {
		color = pressedColor;
	}
	else {
		color = unpressedColor;
	}
	renderInfo.uiRenderInfo.addRectangle(screenRectangle.bot, screenRectangle.top, color, depth);
	return depth + 1;
}

