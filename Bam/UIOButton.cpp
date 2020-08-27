#include "common.h"

#include "UIOButton.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "UIOCallBackParams.h"
#include "Colors.h"

void UIOButton::setColor(glm::vec4 c) {
	this->color = c;
}

void UIOButton::setOnPress(CallBack f) {
	this->onPress = f;
}

void UIOButton::setOnRelease(CallBack f) {
	this->onRelease = f;
}

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
		if (!self_->getScreenRectangle().contains(params.uiState.getCursorPositionScreen())) {
			self_->deactivate();
		}
		return BIND_RESULT::CONTINUE;
	});

	this->addOnHoverBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOButton*>(self_);
		self->down = true;
		self->mousePressOffset = params.uiState.getCursorPositionScreen() - self->getScreenRectangle().getTopLeft();
		return self->onPress(params, self_) | BIND_RESULT::FOCUS | BIND_RESULT::CONSUME;
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

UIOButton::UIOButton(Handle self, UniqueReference<UIOBase, UIOBase> main) :
	UIOButton(self) {
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

int32_t UIOButton::addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) {
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
	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.getBottomLeft(), this->screenRectangle.getTopRight(), c, depth++);
	return depth;
}

