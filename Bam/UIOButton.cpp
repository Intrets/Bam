#include "common.h"

#include "UIOButton.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "UIOCallBackParams.h"
#include "Colors.h"

UIOButton::UIOButton(Handle self) {
	this->onPress = [](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
	{
		return BIND_RESULT::CONTINUE;
	};

	this->onRelease = [](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
	{
		return BIND_RESULT::CONTINUE;
	};

	this->selfHandle = self;

	this->addOnHoverBind({ ControlState::CONTROLS::MOUSE_POS_CHANGED, ControlState::CONTROLSTATE_PRESSED },
						 [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		self_->activate();
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ ControlState::CONTROLS::MOUSE_POS_CHANGED,ControlState::CONTROLSTATE_PRESSED }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (!self_->screenRectangle.contains(params.uiState.getCursorPositionScreen())) {
			self_->deactivate();
		}
		return BIND_RESULT::CONTINUE;
	});

	this->addOnHoverBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED }, [this](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
	{
		this->down = true;
		return this->onPress(state, self_) | BIND_RESULT::FOCUS | BIND_RESULT::CONSUME;
	});

	this->addGlobalBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_RELEASED }, [this](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
	{
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

UIOButton::UIOButton(Handle self, UniqueReference<UIOBase, UIOBase> main, bool shrink) :
	UIOButton(self, std::move(main)) {
	this->shrinkToFit = shrink;
}

ScreenRectangle UIOButton::updateSize(ScreenRectangle newScreenRectangle) {
	if (this->maybeMain) {
		auto temp = this->maybeMain.value()->updateSize(newScreenRectangle);
		if (this->shrinkToFit) {
			newScreenRectangle = temp;
		}
	}
	this->screenRectangle = newScreenRectangle;
	return this->screenRectangle;
}

int32_t UIOButton::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	glm::vec4 c = this->color;
	if (down) {
		c = COLORS::DARKEN(c);
	}
	else if (this->active) {
		c = COLORS::LIGHTEN(c);
	}

	if (this->maybeMain) {
		depth = this->maybeMain.value()->addRenderInfo(gameState, renderInfo, depth + 1);
	}
	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.bot, this->screenRectangle.top, c, depth++);
	return depth;
}

