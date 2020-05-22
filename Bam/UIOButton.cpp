#include "common.h"

#include "UIOButton.h"
#include "GameState.h"
#include "RenderInfo.h"

UIOButton::UIOButton(Handle self) {
	selfHandle = self;

	addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, &onPress);
}

ScreenRectangle UIOButton::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	return screenRectangle;
}

int32_t UIOButton::addRenderInfo(RenderInfo& renderInfo, int32_t depth) {
	glm::vec4 color;
	if (down) {
		color = { 0.5f,0.5f,0.5f,1 };
	}
	else {
		color = glm::vec4(1.0f);
	}
	renderInfo.uiRenderInfo.addRectangle(screenRectangle.bot, screenRectangle.top, color);
	return depth;
}

bool onPress(GameState& gameState, ControlState& controlState, UIOBase* self_) {
	auto* self = static_cast<UIOButton*>(self_);
	if (self->contains(gameState.getPlayerCursorScreenSpace())) {
		self->down = true;
		self->addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_RELEASED }, &onRelease);
		return true;
	}
	return false;
}

bool onRelease(GameState& gameState, ControlState& controlState, UIOBase* self_) {
	auto* self = static_cast<UIOButton*>(self_);
	self->down = false;
	self->addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, &onPress);
	return true;
}
