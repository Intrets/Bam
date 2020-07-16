#pragma once

#include "UIOBase.h"

class UIOButton;
class UIOShell;

class UIOHotbar : public UIOBase
{
public:
	std::vector<UIOButton*> slots;
	std::vector<UIOShell*> slotSize;

	std::array<std::optional<UniqueReference<UIOBase, UIOBase>>, 10> tools;

	int32_t selected = 0;

	UIOHotbar(Handle self);

	virtual CallBackBindResult runGlobalBinds(State& state) override;
	virtual CallBackBindResult runFocussedBinds(State& state) override;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

