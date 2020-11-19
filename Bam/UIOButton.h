#pragma once

#include "UIOBase.h"

#include "Colors.h"

class UIOButton : public UIOBaseSingle
{
private:
	bool shrinkToFit;
	bool highlighted;

	glm::vec2 mousePressOffset;

	bool down = false;
	glm::vec4 color = COLORS::UI::BACKGROUND;

private:
	template<class T>
	friend class UIOConstructer;

	friend class UIOHotbar;

	CallBack onPress;
	CallBack onRelease;

public:
	void setColor(glm::vec4 c);

	void setOnPress(CallBack f);
	void setOnRelease(CallBack f);

	UIOButton(Handle self);
	UIOButton(Handle self, UniqueReference<UIOBase, UIOBase> main);
	UIOButton(Handle self, UniqueReference<UIOBase, UIOBase> main, bool shrink);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

