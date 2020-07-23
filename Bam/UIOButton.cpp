#include "common.h"

#include "UIOButton.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "UIOCallBackParams.h"

UIOButton::UIOButton(Handle self) {
	this->onPress = [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		return BIND_RESULT::CONTINUE;
	};

	this->onRelease = [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		return BIND_RESULT::CONTINUE;
	};

	this->selfHandle = self;

	addGlobalBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (this->contains(state.uiState.getCursorPositionScreen())) {
			this->down = true;
			return this->onPress(state, self_) | BIND_RESULT::FOCUS | BIND_RESULT::CONSUME;
		}
		return BIND_RESULT::CONTINUE;
	});

	addGlobalBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_RELEASED }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		this->down = false;
		return this->onRelease(state, self_) | BIND_RESULT::CONTINUE;
	});
}

ScreenRectangle UIOButton::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;
	return this->screenRectangle;
}

int32_t UIOButton::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	glm::vec4 color;
	if (down) {
		color = this->pressedColor;
	}
	else {
		color = this->unpressedColor;
	}
	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.bot, this->screenRectangle.top, color, depth++);
	return depth;
}

