#pragma once

#include "UIOBase.h"


static bool onPress(GameState& gameState, ControlState& controlState, UIOBase* self);
static bool onRelease(GameState& gameState, ControlState& controlState, UIOBase* self);

class UIOButton : public UIOBase
{
public:
	bool down = false;
	glm::vec4 pressedColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	glm::vec4 unpressedColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	std::vector<CallBack> whileDown;

	UIOButton(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(RenderInfo& renderInfo, int32_t depth) override;
};

