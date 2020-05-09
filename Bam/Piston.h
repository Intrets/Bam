#pragma once

#include "Grouper.h"
#include "Activity.h"

namespace PISTON
{
	typedef enum
	{
		STATIONARY,
		RETRACT,
		EXTEND,
	} DIR;
}

class Piston : public Grouper
{
public:
	int cogTex;
	int ropeTex;
	int headTex;
	int length;

	glm::ivec2 direction;
	MOVEABLE::DIR headDir;

	PISTON::DIR state = PISTON::STATIONARY;

	WeakReference<Activity, Anchor> child;

	Piston(Handle self, GameState& gameState, glm::ivec2 pos, MOVEABLE::DIR dir, bool leaveTraces = true);
	Piston();
	~Piston();

	virtual void forceMoveOrigin(glm::ivec2 d);

	virtual void fillModifyingMap(ModifyerBase& modifier) override;
	virtual void modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) override;

	virtual void doMove(GameState& gameState, MOVEABLE::DIR dir, int pace) override;
	virtual void stopMovement(GameState& gameState) override;
	// Inherited via Activity
	virtual std::stringstream& getMembers(std::stringstream& out) override;
	virtual void doActivityInternal(GameState& gameState, int type, int pace) override;
	virtual ACTIVITY::TYPE getType() override;
	virtual bool canActivity(GameState& gameState, int type, Activity* ignore) override;
	virtual bool canMove(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) override;

	virtual void appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo) override;
	virtual void appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) override;

	// Traces
	//virtual void fillTracesLocalForced(GameState& gameState) override;
	//virtual bool fillTracesUp(GameState& gameState) override;

	//virtual bool canFillTracesLocal(GameState& gameState) override;
	//virtual bool canFillTracesUp(GameState& gameState) override;

	//virtual bool removeTracesForced(GameState& gameState) override;
	//virtual void removeTracesUp(GameState& gameState) override;

	// Activity Traces
	virtual void removeActivityTraces(GameState& gameState) override;
	virtual void leaveActivityTraces(GameState& gameState) override;

	// Moveable Traces
	virtual void removeMoveableTraces(GameState& gameState) override;
	virtual void leaveMoveableTraces(GameState& gameState) override;

	void getGroup(ActivityIgnoringGroup& ignore);
	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;

	virtual bool addChild(WeakReference<Activity, Activity> ref) override;

	virtual void rotateForced(glm::ivec2 center, MOVEABLE::ROT rotation) override;
	virtual bool idleLocal();

	virtual void getTreeMembers(std::vector<Activity*>& members) override;
	virtual bool canFillTracesLocal(GameState& gameState) override;
	virtual void fillTracesLocalForced(GameState& gameState) override;
	virtual void removeTracesLocalForced(GameState& gameState) override;
};
