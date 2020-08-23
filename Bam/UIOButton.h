#pragma once

#include "UIOBase.h"

#include "Colors.h"

class UIOButton : public UIOBase
{
private:
	bool shrinkToFit;
	bool highlighted;

	glm::vec2 mousePressOffset;

	std::optional<UIOBase*> maybeMain;

	bool down = false;
	glm::vec4 color = COLORS::UI::BACKGROUND;

private:
	template<class T>
	friend class UIOConstructer;

	// TODO: refactor UIOHotbar
	friend class UIOHotbar;

	CallBack onPress;
	CallBack onRelease;

public:
	void setColor(glm::vec4 c);

	UIOButton(Handle self);
	UIOButton(Handle self, UniqueReference<UIOBase, UIOBase> main);
	UIOButton(Handle self, UniqueReference<UIOBase, UIOBase> main, bool shrink);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

