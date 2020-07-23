#pragma once

#include "UIOBase.h"

class UIOButton : public UIOBase
{
public:
	CallBack onPress;
	CallBack onRelease;

	bool down = false;
	glm::vec4 pressedColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	glm::vec4 unpressedColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	UIOButton(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

