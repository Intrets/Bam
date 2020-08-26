#pragma once

#include "UIOBase.h"

class UIOButton;
class UIOShell;

class UIOHotbar : public UIOBase
{
private:
	std::array<std::optional<std::function<void(UIOCallBackParams&)>>, 10> tools;

public:
	UIOHotbar(Handle self);

	void activateTool(int32_t slot, UIOCallBackParams& params);

	void addTool(int32_t slot, std::function<void(UIOCallBackParams& params)> f);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

