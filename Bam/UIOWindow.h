#pragma once

#include "UIOBase.h"
#include "UIOButton.h"

class UIOWindow : public UIOBase
{
public:
	enum TYPE
	{
		MINIMISE = 1 << 0,
		CLOSE = 1 << 1,
		RESIZE = 1 << 2,
		RESIZEHORIZONTAL = 1 << 3,
		RESIZEVERTICAL = 1 << 4,
		MOVE = 1 << 5
	};


	UIOBase* main;
	UIOBase* topBarPtr;

	bool minimized = false;

	UIOWindow(Handle self, UniqueReference<UIOBase, UIOBase> main_);
	UIOWindow(Handle self);
	UIOWindow() = default;
	virtual ~UIOWindow() = default;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) override;

	virtual CallBackBindResult runOnHoverBinds(State& state) override;
	virtual CallBackBindResult runFocussedBinds(State& state) override;
	virtual CallBackBindResult runActiveBinds(State& state) override;
	virtual CallBackBindResult runGlobalBinds(State& state) override;
};

