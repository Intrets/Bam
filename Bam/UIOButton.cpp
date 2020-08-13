#include "common.h"

#include "UIOButton.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "UIOCallBackParams.h"
#include "Colors.h"

UIOButton::UIOButton(Handle self) {
	this->onPress = [](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		return BIND_RESULT::CONTINUE;
	};

	this->onRelease = [](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		return BIND_RESULT::CONTINUE;
	};

	this->selfHandle = self;

	this->addOnHoverBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED }, [this](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		this->down = true;
		return this->onPress(state, self_) | BIND_RESULT::FOCUS | BIND_RESULT::CONSUME;
	});

	this->addGlobalBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_RELEASED }, [this](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (this->down) {
			this->down = false;
			return this->onRelease(state, self_) | BIND_RESULT::CONTINUE;
		}
		else {
			return BIND_RESULT::CONTINUE;
		}
	});
}

UIOButton::UIOButton(Handle self, UniqueReference<UIOBase, UIOBase> main) : UIOButton(self) {
	this->maybeMain = main.get();
	this->addElement(std::move(main));
}

ScreenRectangle UIOButton::updateSize(ScreenRectangle newScreenRectangle) {
	if (this->maybeMain) {
		this->maybeMain.value()->updateSize(newScreenRectangle);
	}
	this->screenRectangle = newScreenRectangle;
	return this->screenRectangle;
}

int32_t UIOButton::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	glm::vec4 c = this->color;
	if (down) {
		c = COLORS::DARKEN(c);
	}
	if (this->maybeMain) {
		depth = this->maybeMain.value()->addRenderInfo(gameState, renderInfo, depth + 1);
	}
	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.bot, this->screenRectangle.top, c, depth++);
	return depth;
}

