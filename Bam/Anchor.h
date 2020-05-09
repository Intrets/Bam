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

	virtual void forceMoveOrigin(glm::ivec2 d);

	virtual void rotateForced(glm::ivec2 center, MOVEABLE::ROT rotation) override;

	virtual void appendStaticRenderInfo(GameState & gameState, StaticWorldRenderInfo & staticWorldRenderInfo) override;
	virtual void appendSelectionInfo(GameState & gameState, RenderInfo & renderInfo) override;

	virtual bool canMoveLocal(GameState & gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) override;
	//virtual void applyMoveLocalForced(GameState & gameState, MOVEABLE::DIR dir, int pace) override;
	//virtual void stopMovement(GameState& gameState) override;

	virtual bool canActivityLocal(GameState & gameState, int type) override;
	//virtual void doActivityInternal(GameState & gameState, int type, int pace) override;

	virtual void removeMoveableTracesLocal(GameState & gameState) override;
	virtual void leaveMoveableTracesLocal(GameState & gameState) override;

	virtual void removeActivityTracesLocal(GameState & gameState) override;
	virtual void leaveActivityTracesLocal(GameState & gameState) override;

	virtual void save(Saver & saver) override;
	virtual bool load(Loader& loader) override;
	
	void modifyMember(GameState & gameState, std::string & name, std::vector<std::string>& value) override;
	virtual void fillModifyingMap(ModifyerBase& modifyer) override;
	virtual std::stringstream& getMembers(std::stringstream& out) override;

	virtual ACTIVITY::TYPE getType() override;

	// Inherited via Activity
	virtual void getTreeMembers(std::vector<Activity*>& members) override;
	virtual bool canFillTracesLocal(GameState& gameState) override;
	virtual void fillTracesLocalForced(GameState& gameState) override;
	virtual void removeTracesLocalForced(GameState& gameState) override;

	// Inherited via Activity
	virtual void applyActivityLocalForced(GameState& gameState, int type, int pace) override;
};

