#pragma once

#include "UIOBase.h"

class UIOProxy : public UIOBase
{
public:
	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

	UIOProxy(Handle self, WeakReference<UIOBase, UIOBase> m);

	virtual CallBackBindResult runGlobalBinds(State& state) override;
	virtual CallBackBindResult runFocussedBinds(State& state) override;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth);

private:
	ManagedReference<UIOBase, UIOBase> main;
};

