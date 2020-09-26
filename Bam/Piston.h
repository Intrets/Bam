#pragma once

#include "Grouper.h"
#include "Activity.h"
#include "ActivityMaterial.h"
#include "Block.h"

#include "Enums.h"

class Piston : public SingleGrouper
{
private:
	int32_t length;

	friend class ACTIVITYCOPIER;

private:
	ShapedBlock headBlock;
	ShapedBlock shaftBlock;
	ShapedBlock baseBlock;

public:
	Piston(Handle self, glm::ivec2 pos, ACTIVITY::DIR dir);
	Piston() = default;
	virtual ~Piston() = default;

	// Placement
	virtual void rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) override;

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) override;
	virtual void forceChangeActivityState(int32_t type) override;

	// Moveable
	virtual bool canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) override;

	// Traces Placement
	virtual bool canFillTracesLocal(GameState& gameState) override;
	virtual void fillTracesLocalForced(GameState& gameState) override;
	virtual void removeTracesLocalForced(GameState& gameState) override;

	// Traces Activity
	virtual void preActivityStopLocal(GameState& gameState) override;

	// Traces Moveable
	virtual void preMoveableStopLocal(GameState& gameState) override;
	virtual void postMoveableStartLocal(GameState& gameState) override;

	// Serial
	virtual ACTIVITY::TYPE getType() override;
	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;

	// Render
	virtual void appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) override;
	virtual void appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) override;
};

