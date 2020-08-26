#pragma once

#include "UIOBase.h"

class UIOHideable : public UIOBase
{
private:
	bool hidden;

	// if the parent container should be brought to focus
	bool shouldFocus;

	// if the UIOHideable should bring the parent back to focus on being shown again
	bool focusOnShow = true;

public:
	UIOBase* main;

	UIOHideable(Handle selfHandle, UniqueReference<UIOBase, UIOBase> main_, bool focusOnShow);

	void hide();
	void show();

	virtual CallBackBindResult runGlobalBinds(State& state) override;
	virtual CallBackBindResult runFocussedBinds(State& state) override;
	virtual CallBackBindResult runOnHoverBinds(State& state) override;
	virtual CallBackBindResult runActiveBinds(State& state) override;
	virtual CallBackBindResult runGameWorldBinds(State& state) override;

	virtual int32_t addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) override;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

