#pragma once

#include "ActivitySelector.h"

class GameState;
struct RenderInfo;

class ActivityEditor : public ActivitySelector
{
protected:
	virtual void appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) override;
};

