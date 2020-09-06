#pragma once

#include "Activity.h"
#include "Block.h"

class Platform : public Activity
{
private:
	glm::ivec2 size;

	std::vector<std::vector<Block>> blocks;
	std::array<std::vector<glm::ivec2>, 4> blockedDirections;

	void calculateBlockedDirections();

public:
	Platform(Handle self, GameState& gameState, glm::ivec2 size, glm::ivec2 pos);
	Platform() = default;
	~Platform() = default;

	// Placement
	virtual void rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) override;

	// Activity
	virtual bool canActivityLocal(GameState & gameState, int32_t type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) override;

	// Moveable
	virtual bool canMoveLocal(GameState & gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) override;

	// Traces Placement
	virtual bool canFillTracesLocal(GameState& gameState) override;
	virtual void fillTracesLocalForced(GameState& gameState) override;
	virtual void removeTracesLocalForced(GameState& gameState) override;

	// Traces Activity
	virtual void removeActivityTracesLocal(GameState & gameState) override;
	virtual void leaveActivityTracesLocal(GameState & gameState) override;

	// Traces Moveable
	virtual void removeMoveableTracesLocal(GameState & gameState) override;
	virtual void leaveMoveableTracesLocal(GameState & gameState) override;

	// Tree Information
	virtual void impl_getTreeMembers(std::vector<Activity*>& members) override;
	virtual void getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) override;

	// Serial
	virtual void save(Saver & saver) override;
	virtual bool load(Loader& loader) override;

	// Render
	virtual void appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) override;
	virtual void appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo & staticWorldRenderInfo) override;

	virtual ACTIVITY::TYPE getType() override;
};

