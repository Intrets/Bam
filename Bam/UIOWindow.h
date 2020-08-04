#pragma once

#include "UIOBase.h"
#include "UIOButton.h"

class UIOWindow : public UIOBase
{
public:
	UIOBase* main;
	UIOBase* topBarPtr;

	bool minimized;

	glm::vec2 mousePressedPosOffset;

	UIOWindow(Handle self, UniqueReference<UIOBase, UIOBase> main_, std::string title);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

	virtual CallBackBindResult runGlobalBinds(State& state) override;
	virtual CallBackBindResult runFocussedBinds(State& state) override;
};

