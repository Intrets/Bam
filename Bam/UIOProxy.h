#pragma once

#include "UIOBase.h"

class UIOProxy : public UIOBase
{
public:
	ManagedReference<UIOBase, UIOBase> main;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

	UIOProxy(Handle self);
	
	virtual CallBackBindResult runGlobalBinds(State& state) override;
	virtual CallBackBindResult runFocussedBinds(State& state) override;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth);
};

