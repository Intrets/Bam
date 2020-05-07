#pragma once

#include "ModifyingInterface.h"
#include "ReferenceManager.h"

#include <initializer_list>

class ActivityIgnoringGroup;
class Loader;
class Saver;
class GameState;
struct StaticWorldRenderInfo;
struct RenderInfo;

namespace MOVEABLE
{
	typedef enum
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
		STATIONARY,
	} DIR;

	const std::vector<std::string> DIR_NAMES{
		"up",
		"right",
		"down",
		"left",
		"stationary"
	};

	static std::array<DIR, 5> REV_DIR{
		DOWN,
		LEFT,
		UP,
		RIGHT,
		STATIONARY,
	};

	// TODO: does this break?
	static std::array<glm::ivec2, 5> DIRECTION{
			glm::ivec2(0, 1),
			glm::ivec2(1, 0),
			glm::ivec2(0, -1),
			glm::ivec2(-1, 0),
			glm::ivec2(0, 0)
	};
}

// TODO: better runtime type system??
namespace ACTIVITY
{
	typedef enum
	{
		ANCHOR,
		MOVER,
		PLATFORM,
		PISTON,
		SINGLEPLATFORM,
		BREAKER,
		GRABBER,
		PLANT,
		_MAX,
	} TYPE;

	const std::vector<std::string> TYPE_NAMES{
		"anchor",
		"mover",
		"platform",
		"piston",
		"single_platform",
		"breaker",
		"grabber",
		"plant",
		"_MAX_SHOULD_NOT_SEE_THIS",
	};
}

class Anchor;

class Activity : public ModifyingInterface
{
private:
	virtual void doActivityInternal(GameState& gameState, int type, int pace) = 0;

	Handle getRootHandle();

protected:
	glm::vec2 getMovingOrigin(GameState& gameState);

	int activityPace = 10;
	int activityTickStart = 0;
	int activityType = 0;
	bool active = false;

	int movingPace = 20;
	int movingTickStart = 0;
	MOVEABLE::DIR movementDirection = MOVEABLE::STATIONARY;
	bool moving = false;

	glm::ivec2 origin;

public:
	WeakReference<Activity, Anchor> parentRef;
	Handle selfHandle;

	void forceOrigin(glm::ivec2 origin_) { origin = origin_; };
	glm::ivec2 getOrigin() { return origin; };

	virtual ACTIVITY::TYPE getType() = 0;
	Handle getHandle() { return selfHandle; };

	virtual bool canActivity(GameState& gameState, int type, Activity* ignore) = 0;
	virtual bool applyActivity(GameState& gameState, int type, int pace);
	virtual void doActivity(GameState& gameState, int type, int pace);
	virtual void stopActivity(GameState& gameState);

	virtual bool canMove(GameState & gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) = 0;
	virtual bool applyRootMove(GameState& gameState, MOVEABLE::DIR dir, int pace);
	virtual bool applyCurrentMove(GameState& gameState, MOVEABLE::DIR dir, int pace);
	virtual void doMove(GameState& gameState, MOVEABLE::DIR dir, int pace);
	virtual void stopMovement(GameState& gameState);

	virtual void appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo) = 0;
	virtual void appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) = 0;

	virtual bool fillTraces(GameState& gameState) = 0;
	virtual bool removeTraces(GameState& gameState) = 0;

	virtual void removeActivityTraces(GameState& gameState) = 0;
	virtual void leaveActivityTraces(GameState& gameState) = 0;

	virtual void removeMoveableTraces(GameState& gameState) = 0;
	virtual void leaveMoveableTraces(GameState& gameState) = 0;

	virtual void getGroup(ActivityIgnoringGroup& ignore);
	WeakReference<Activity, Activity> getRoot();

	virtual void save(Saver& saver);
	virtual bool load(Loader& loader);

	virtual void fillModifyingMap(ModifyerBase& modifyer) override;

	using ModifyingInterface::getSimpleInfo;
	virtual std::ostream& getSimpleInfo(std::ostream& out) override;

	Activity() = default;
	Activity(Handle self, glm::ivec2 p) : selfHandle(self), origin(p), parentRef(0) {};
	virtual ~Activity() = default;

	glm::ivec2 getDirection(int i) {
		return MOVEABLE::DIRECTION[i];
	}
};

