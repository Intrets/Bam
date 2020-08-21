#pragma once

#include "UIOBase.h"

#include "Colors.h"

class UIOButton : public UIOBase
{
private:
	bool shrinkToFit;
	bool highlighted;

public:
	CallBack onPress;
	CallBack onRelease;

	glm::vec2 mousePressOffset;
	
	std::optional<UIOBase*> maybeMain;

	bool down = false;

	glm::vec4 color = COLORS::UI::BACKGROUND;

	UIOButton(Handle self);
	UIOButton(Handle self, UniqueReference<UIOBase, UIOBase> main);
	UIOButton(Handle self, UniqueReference<UIOBase, UIOBase> main, bool shrink);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

