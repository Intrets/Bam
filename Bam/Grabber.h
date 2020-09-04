#pragma once

#include "SingleBlockActivity.h"
#include "Block.h"

namespace GRABBER
{
	enum STATE
	{
		RELEASED,
		GRABBED,
	};
}
 
class GameState;

class Grabber : public Activity
{
private:
	std::array<int32_t, 4> textures;
	std::array<int32_t, 4> textures_a;

	Block block;
	ACTIVITY::DIR dir = ACTIVITY::DIR::RIGHT;

	glm::ivec2 getGrabbedOffset() const;
	glm::ivec2 getGrabbedPos() const;

public:
	Grabber();
	Grabber(Handle self, glm::ivec2 pos);
	virtual ~Grabber() = default;

	// Placement
	virtual void rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) override;

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) override;

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

	// Tree Information
	virtual void getTreeMembers(std::vector<Activity*>& members) override;
	virtual void getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) override;

	// Serial
	virtual ACTIVITY::TYPE getType() override;
	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;

	// Render
	virtual void appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) override;
	virtual void appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) override;
};

