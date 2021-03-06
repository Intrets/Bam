#include "common.h"

#include "UIOButton.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "PlayerState.h"
#include "Colors.h"
#include "UIOEmpty.h"

void UIOButton::setColor(glm::vec4 c) {
	this->color = c;
}

void UIOButton::setOnPress(CallBack f) {
	this->onPress = f;
}

void UIOButton::setOnRelease(CallBack f) {
	this->onRelease = f;
}

void UIOButton::setShrinkToFit(bool b) {
	this->shrinkToFit = b;
}

bool UIOButton::isDown() {
	return this->down;
}

void UIOButton::dePress() {
	this->down = false;
}

glm::vec2 const& UIOButton::getMousePressOffset() const {
	return this->mousePressOffset;
}

UIOButton::UIOButton(Handle self) {
	this->onPress = [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		return BIND::RESULT::CONTINUE;
	};

	this->onRelease = [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		return BIND::RESULT::CONTINUE;
	};

	this->selfHandle = self;

	this->addOnHoverBind({ CONTROL::KEY::MOUSE_POS_CHANGED_TOPLEVEL, CONTROL::STATE::PRESSED },
						 [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		self_->activate();
		return BIND::RESULT::CONSUME;
	});

	this->addGlobalBind({ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		if (!self_->getScreenRectangle().contains(playerState.uiState.getCursorPositionScreen()) ||
			!playerState.controlState.activated({ CONTROL::KEY::MOUSE_POS_CHANGED_TOPLEVEL })) {
			self_->deactivate();
		}
		return BIND::RESULT::CONTINUE;
	});

	this->addOnHoverBind({ CONTROL::KEY::ACTION0, CONTROL::STATE::PRESSED }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOButton*>(self_);
		self->down = true;
		self->mousePressOffset = playerState.uiState.getCursorPositionScreen() - self->getScreenRectangle().getTopLeft();
		return self->onPress(playerState, self_) | BIND::RESULT::FOCUS | BIND::RESULT::CONSUME;
	});

	this->addGlobalBind({ CONTROL::KEY::ACTION0, CONTROL::STATE::RELEASED }, [this](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		if (this->down) {
			this->down = false;

			if (self_->getScreenRectangle().contains(playerState.uiState.getCursorPositionScreen())) {
				return this->onRelease(playerState, self_) | BIND::RESULT::CONTINUE;
			}
			else {
				return BIND::RESULT::CONTINUE;
			}
		}
		else {
			return BIND::RESULT::CONTINUE;
		}
	});
}

ScreenRectangle UIOButton::updateSize(ScreenRectangle newScreenRectangle) {
	auto temp = this->main.get()->updateSize(newScreenRectangle);
	if (this->shrinkToFit) {
		newScreenRectangle = temp;
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

	depth = this->main.get()->addRenderInfo(gameState, renderInfo, depth + 1);
	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.getBottomLeft(), this->screenRectangle.getTopRight(), c, depth++);
	return depth;
}

