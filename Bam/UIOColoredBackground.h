#pragma once

#include "UIOBase.h"
#include "Colors.h"

class UIOColoredBackground : public UIOBaseSingle
{
public:
	glm::vec4 color = COLORS::UI::BACKGROUND;

	UIOColoredBackground(Handle self);
	UIOColoredBackground(Handle self, UniqueReference<UIOBase, UIOBase> main_);
	UIOColoredBackground(Handle self, glm::vec4 c, UniqueReference<UIOBase, UIOBase> main_);

	UIOColoredBackground() = default;
	~UIOColoredBackground() = default;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

