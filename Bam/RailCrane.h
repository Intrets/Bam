#pragma once

#include "Grouper.h"

namespace RAILCRANE{
	enum class DIR
	{
		STATIONARY,
		LEFT,
		RIGHT,
	};
}

class RailCrane : public SingleGrouper
{
public:
	int32_t supportTex;
	int32_t shaftTex;
	int32_t anchorTex;

	ACTIVITY::DIR orientation;
	RAILCRANE::DIR anchorDirection;
	int32_t length;
	int32_t anchorIndexPos;

	RailCrane(Handle self, GameState& gameState, glm::ivec2 pos);
	RailCrane() = default;
	virtual	~RailCrane() = default;

	// Placement
	virtual void rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) override;

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) override;
	virtual void forceChangeActivityState(int32_t s) override;

	// Moveable
	virtual bool canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) override;

	// Traces Placement
	virtual bool canFillTracesLocal(GameState& gameState) override;
	virtual void fillTracesLocalForced(GameState& gameState) override;
	virtual void removeTracesLocalForced(GameState& gameState) override;

	// Traces Activity
	virtual void removeActivityTracesLocal(GameState& gameState) override;
	virtual void leaveActivityTracesLocal(GameState& gameState) override;

	// Traces Moveable
	virtual void removeMoveableTracesLocal(GameState& gameState) override;
	virtual void leaveMoveableTracesLocal(GameState& gameState) override;

	// Serial
	virtual ACTIVITY::TYPE getType() override;
	virtual void save(Saver& saver);
	virtual bool load(Loader& loader);

	// Render
	virtual void appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) override;
	virtual void appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) override;
};

