#pragma once

#include "Activity.h"
#include "GameState.h"
#include "ReferenceManager.h"

class Anchor : public Activity
{
public:
	std::vector<WeakReference<Activity, Activity>> children;

	void addChild(Handle h);

	Anchor();
	Anchor(Handle self);
	Anchor(Handle self, GameState& gameState);
	~Anchor();

	virtual void appendStaticRenderInfo(GameState & gameState, StaticWorldRenderInfo & staticWorldRenderInfo) override;
	virtual void appendSelectionInfo(GameState & gameState, RenderInfo & renderInfo) override;

	virtual bool canMove(GameState & gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) override;
	virtual void doMove(GameState & gameState, MOVEABLE::DIR dir, int pace) override;
	virtual void stopMovement(GameState& gameState) override;

	virtual bool canActivity(GameState & gameState, int type, Activity * ignore) override;
	virtual void doActivityInternal(GameState & gameState, int type, int pace) override;

	virtual void fillTraces(GameState& gameState) override;
	virtual void removeMoveableTraces(GameState & gameState) override;
	virtual void leaveMoveableTraces(GameState & gameState) override;

	virtual void removeActivityTraces(GameState & gameState) override;
	virtual void leaveActivityTraces(GameState & gameState) override;

	virtual void getGroup(ActivityIgnoringGroup& ignore) override;

	virtual void save(Saver & saver) override;
	virtual bool load(Loader& loader) override;
	
	void modifyMember(GameState & gameState, std::string & name, std::vector<std::string>& value) override;
	virtual void fillModifyingMap(ModifyerBase& modifyer) override;
	virtual std::stringstream& getMembers(std::stringstream& out) override;

	virtual ACTIVITY::TYPE getType() override;

};
