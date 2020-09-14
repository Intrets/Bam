#pragma once

#include "UIOBase.h"

class UIOButton;
class UIOShell;
class UIOTextDisplay;

class UIOHotbar : public UIOBase
{
private:
	std::array<std::optional<std::function<void(UIOCallBackParams&)>>, 10> tools;

	std::array<UIOTextDisplay*, 10> toolTexts;

public:
	UIOHotbar(Handle self);
	virtual ~UIOHotbar() = default;

	void activateTool(int32_t slot, UIOCallBackParams& params);

	void setTool(int32_t slot, std::string name, std::function<void(UIOCallBackParams& params)> f);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

