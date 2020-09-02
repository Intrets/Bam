#pragma once

#include "Activity.h"
#include "ReferenceManager.h"
#include "Grouper.h"

class GameState;

class Anchor : public GrouperBase
{
private:
	std::vector<WeakReference<Activity, Activity>> children;

public:
	std::vector<WeakReference<Activity, Activity>>& getChildren();

	// Adds activity to this Anchor and sets the parentRef to this Anchor
	virtual bool addChild(WeakReference<Activity, Activity> ref) override;

	// returns if this Anchor has no children and has to be cleaned up
	virtual bool removeChild(WeakReference<Activity, Activity> ref) override;

	virtual bool hasChild() const override;

	Anchor();
	Anchor(Handle self);
	~Anchor();

	// Placement
	virtual void rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) override;

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) override;

	// Moveable
	virtual bool canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) override;

	// Traces Placement
	virtual bool canFillTracesLocal(GameState& gameState) override;

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

