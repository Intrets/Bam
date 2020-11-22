#pragma once

#include "UIOBase.h"
#include "Colors.h"

class UIOColoredBackground : public UIOBaseSingle
{
public:
	glm::vec4 color = COLORS::UI::BACKGROUND;

	UIOColoredBackground(Handle self);
	~UIOColoredBackground() = default;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

