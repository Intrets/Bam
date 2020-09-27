#pragma once

#include "Activity.h"
#include "ActivityMaterial.h"

class GameState;

class SingleBlockActivity : public Activity
{
public:
	SingleBlockActivity() = default;
	SingleBlockActivity(Handle self, glm::ivec2 pos);

	virtual ~SingleBlockActivity() = default;

	// Placement
	virtual void rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) override;

	// Moveable
	virtual bool canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) override;

	// Traces Placement
	virtual bool canFillTracesLocal(GameState& gameState) override;
	virtual void fillTracesLocalForced(GameState& gameState) override;
	virtual void removeTracesLocalForced(GameState& gameState) override;

	// Traces Moveable
	virtual void preMoveableStopLocal(GameState& gameState) override;
	virtual void postMoveableStartLocal(GameState& gameState) override;

	// Tree Informations

	// non-cached base implementation
	virtual void impl_getTreeMembers(std::vector<Activity*>& members) override;
	// non-cached base implementation
	virtual void impl_getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) override;

	// Render
	virtual void appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) override;
	virtual void appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) override;

	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;
};

