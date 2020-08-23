#pragma once

#include "UIOBase.h"
#include "Colors.h"

class UIOColoredBackground : public UIOBase
{
public:
	glm::vec4 color = COLORS::UI::BACKGROUND;

	UIOBase* main;

	UIOColoredBackground(Handle self, UniqueReference<UIOBase, UIOBase> main_);
	UIOColoredBackground(Handle self, glm::vec4 c, UniqueReference<UIOBase, UIOBase> main_);

	UIOColoredBackground() = default;
	~UIOColoredBackground() = default;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

