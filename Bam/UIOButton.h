#pragma once

#include "UIOBase.h"

class UIOButton : public UIOBase
{
private:
	bool shrinkToFit;
public:
	CallBack onPress;
	CallBack onRelease;
	
	std::optional<UIOBase*> maybeMain;

	bool down = false;
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	UIOButton(Handle self);
	UIOButton(Handle self, UniqueReference<UIOBase, UIOBase> main);
	UIOButton(Handle self, UniqueReference<UIOBase, UIOBase> main, bool shrink);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

