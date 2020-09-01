#pragma once

#include "ReferenceManager.h"

#include <initializer_list>

class ActivityIgnoringGroup;
class GrouperBase;
class Loader;
class Saver;
class GameState;
struct StaticWorldRenderInfo;
struct RenderInfo;

// TODO: better runtime type system??
namespace ACTIVITY
{
	const std::vector<std::string> TYPE_NAMES{
		"anchor",
		"mover",
		"platform",
		"piston",
		"single_platform",
		"breaker",
		"grabber",
		"plant",
		"activator",
		"railcrane",
		"lua",
		"_MAX_SHOULD_NOT_SEE_THIS",
	};
}

class Anchor;

class Activity 
{
public:
	enum DIR
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
		STATIONARY,
	};

	static Activity::DIR CLOCKWISE(Activity::DIR dir) {
		return static_cast<Activity::DIR>((static_cast<int32_t>(dir) + 1) % 4);
	}

	static Activity::DIR COUNTERWISE(Activity::DIR dir) {
		return static_cast<Activity::DIR>((static_cast<int32_t>(dir) + 3) % 4);
	}
	
	static std::string GET_NAME(DIR dir) {
		switch (dir) {
			case Activity::DIR::UP:
				return "up";
				break;
			case Activity::DIR::RIGHT:
				return "right";
				break;
			case Activity::DIR::DOWN:
				return "down";
				break;
			case Activity::DIR::LEFT:
				return "left";
				break;
			case Activity::DIR::STATIONARY:
				return "stationary";
				break;
			default:
				assert(0);
				return "";
				break;
		}
	}

	static DIR FLIP(DIR dir) {
		switch (dir) {
			case DIR::UP:
				return Activity::DIR::DOWN;
				break;
			case DIR::RIGHT:
				return Activity::DIR::LEFT;
				break;
			case DIR::DOWN:
				return Activity::DIR::UP;
				break;
			case DIR::LEFT:
				return Activity::DIR::RIGHT;
				break;
			case DIR::STATIONARY:
				return Activity::DIR::STATIONARY;
				break;
			default:
				assert(0);
				return Activity::DIR::STATIONARY;
				break;
		}
	}

	static glm::ivec2 getDirection(Activity::DIR dir) {
		switch (dir) {
			case Activity::DIR::UP:
				return glm::ivec2(0, 1);
				break;
			case Activity::DIR::RIGHT:
				return glm::ivec2(1, 0);
				break;
			case Activity::DIR::DOWN:
				return glm::ivec2(0, -1);
				break;
			case Activity::DIR::LEFT:
				return glm::ivec2(-1, 0);
				break;
			case Activity::DIR::STATIONARY:
				return glm::ivec2(0, 0);
				break;
			default:
				assert(0);
				return glm::ivec2(0, 0);
				break;
		}
	}

	enum class ROT
	{
		CLOCKWISE,
		COUNTERCLOCKWISE,
	};

	enum class TYPE
	{
		ANCHOR,
		MOVER,
		PLATFORM,
		PISTON,
		SINGLEPLATFORM,
		BREAKER,
		GRABBER,
		PLANT,
		ACTIVATOR,
		RAILCRANE,
		LUA,
		_MAX,
	};
private:
	Handle getRootHandle() const;

protected:
	glm::vec2 getMovingOrigin(GameState const& gameState) const;

	int32_t activityPace = 10;
	int32_t activityTickStart = 0;
	int32_t activityType = 0;
	bool active = false;

	int32_t movingPace = 20;
	int32_t movingTickStart = 0;
	DIR movementDirection = DIR::STATIONARY;
	bool moving = false;

	bool inWorld = false;

	glm::ivec2 origin;

public:
	WeakReference<Activity, GrouperBase> parentRef;
	Handle selfHandle;

	virtual glm::ivec2 getOrigin() { return origin; };
	Handle getHandle() { return selfHandle; };

	bool idleUp();
	virtual bool idleLocal();
	virtual bool moveableIdleLocal();
	virtual bool activityIdleLocal();

	float getMovementScale(int32_t  tick);
	float getActivityScale(int32_t  tick);

	// Constructors
	Activity() = default;
	Activity(Handle self, glm::ivec2 p) : selfHandle(self), origin(p), parentRef(0) {};
	virtual ~Activity() = default;

	// Placement 
	virtual void rotateForcedLocal(glm::ivec2 center, Activity::ROT rotation) = 0;
	void rotateForcedUp(glm::ivec2 center, Activity::ROT rotation);
	void forceMoveOriginUp(glm::ivec2 d);
	void forceMoveOriginLocal(glm::ivec2 d) { origin += d; };
	void disconnectFromParent();

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) = 0;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) = 0;
	bool applyActivityLocal(GameState& gameState, int32_t type, int32_t pace);
	void stopActivity(GameState& gameState);

	// Forcefully change the activity state, only use when activity is not in static world.
	// The type is e.g. retracting or extending a piston
	virtual void forceChangeActivityState(int32_t type);

	// Moveable
	virtual bool canMoveLocal(GameState& gameState, Activity::DIR dir, ActivityIgnoringGroup& ignore) = 0;
	virtual void applyMoveLocalForced(GameState& gameState, Activity::DIR dir, int32_t pace);
	bool canMoveUp(GameState& gameState, Activity::DIR dir);
	bool canMoveUp(GameState& gameState, Activity::DIR dir, std::vector<Activity*>& extraIgnore);
	bool applyMoveUp(GameState& gameState, Activity::DIR dir, int32_t pace);
	void applyMoveUpForced(GameState& gameState, Activity::DIR dir, int32_t pace);
	bool applyMoveRoot(GameState& gameState, Activity::DIR dir, int32_t pace);
	void stopMovement(GameState& gameState);

	// Traces Placement
	virtual bool canFillTracesLocal(GameState& gameState) = 0;
	virtual void fillTracesLocalForced(GameState& gameState) = 0;
	virtual void removeTracesLocalForced(GameState& gameState) = 0;

	void fillTracesUpForced(GameState& gameState);
	bool fillTracesUp(GameState& gameState);

	void removeTracesUpForced(GameState& gameState);
	bool removeTracesUp(GameState& gameState);

	// Traces Activity
	virtual void removeActivityTracesLocal(GameState& gameState) = 0;
	virtual void leaveActivityTracesLocal(GameState& gameState) = 0;

	// Traces Moveable
	virtual void removeMoveableTracesLocal(GameState& gameState) = 0;
	virtual void leaveMoveableTracesLocal(GameState& gameState) = 0;

	// Tree Informations
	virtual void getTreeMembers(std::vector<Activity*>& members) = 0;
	virtual void getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) = 0;
	WeakReference<Activity, Activity> getRoot() const;

	// Serial
	virtual TYPE getType() = 0;
	std::string getTypeName();
	virtual void save(Saver& saver);
	virtual bool load(Loader& loader);

	// Render
	virtual void appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) = 0;
	virtual void appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) = 0;
};

