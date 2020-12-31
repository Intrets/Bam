#pragma once

#include "UIOBase.h"

class UIOHideable : public UIOBaseSingle
{
private:
	bool hidden = false;

	// if the parent container should be brought to focus
	bool shouldFocus = false;

	// if the UIOHideable should bring the parent back to focus on being shown again
	bool focusOnShow = true;

public:
	UIOHideable(Handle self);
	virtual ~UIOHideable() = default;

	void hide();
	void show();

	virtual CallBackBindResult runGlobalBinds(PlayerState& state) override;
	virtual CallBackBindResult runFocussedBinds(PlayerState& state) override;
	virtual CallBackBindResult runOnHoverBinds(PlayerState& state) override;
	virtual CallBackBindResult runActiveBinds(PlayerState& state) override;
	virtual CallBackBindResult runGameWorldBinds(PlayerState& state) override;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

