#pragma once

#include "Activity.h"
#include "GameState.h"
#include "ReferenceManager.h"

class Anchor : public Activity
{
public:
	std::vector<WeakReference<Activity, Activity>> children;

	void addChild(Handle h);

	// returns if this Anchor has no children and has to be cleaned up
	bool removeChild(Handle h);

	Anchor();
	Anchor(Handle self);
	Anchor(Handle self, GameState& gameState);
	~Anchor();

	// Placement
	virtual void rotateForcedLocal(glm::ivec2 center, MOVEABLE::ROT rotation) override;

	// Activity
	virtual bool canActivityLocal(GameState & gameState, int type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int type, int pace) override;

	// Moveable
	virtual bool canMoveLocal(GameState & gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) override;

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
	virtual void getTreeMembers(std::vector<Activity*>& members) override;

	// Serial
	virtual ACTIVITY::TYPE getType() override;
	virtual void save(Saver & saver) override;
	virtual bool load(Loader& loader) override;

	// Render
	virtual void appendStaticRenderInfo(GameState & gameState, StaticWorldRenderInfo & staticWorldRenderInfo) override;
	virtual void appendSelectionInfo(GameState & gameState, RenderInfo & renderInfo, glm::vec4 color) override;

	// Modifying
	void modifyMember(GameState & gameState, std::string & name, std::vector<std::string>& value) override;
	virtual void fillModifyingMap(ModifyerBase& modifyer) override;
	virtual std::stringstream& getMembers(std::stringstream& out) override;
};

