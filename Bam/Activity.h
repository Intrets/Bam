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
		RAILCRANE,
		LUA,
		_MAX,
	};

	const std::vector<std::string> TYPE_NAMES{
		"anchor",
		"mover",
		"platform",
		"piston",
		"single_platform",
		"breaker",
		"grabber",
		"plant",
		"railcrane",
		"lua",
		"_MAX_SHOULD_NOT_SEE_THIS",
	};

	enum DIR
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
		STATIONARY,
	};

	enum class ROT
	{
		CLOCKWISE,
		COUNTERCLOCKWISE,
	};

	inline ACTIVITY::DIR CLOCKWISEROT(ACTIVITY::DIR dir) {
		return static_cast<ACTIVITY::DIR>((static_cast<int32_t>(dir) + 1) % 4);
	}

	inline ACTIVITY::DIR COUNTERWISEROT(ACTIVITY::DIR dir) {
		return static_cast<ACTIVITY::DIR>((static_cast<int32_t>(dir) + 3) % 4);
	}

	inline ACTIVITY::DIR ROTATE(ACTIVITY::ROT rot, ACTIVITY::DIR dir) {
		if (rot == ACTIVITY::ROT::CLOCKWISE) {
			return CLOCKWISEROT(dir);
		}
		else {
			return COUNTERWISEROT(dir);
		}
	}

	inline std::string GET_NAME(ACTIVITY::DIR dir) {
		switch (dir) {
			case ACTIVITY::DIR::UP:
				return "up";
				break;
			case ACTIVITY::DIR::RIGHT:
				return "right";
				break;
			case ACTIVITY::DIR::DOWN:
				return "down";
				break;
			case ACTIVITY::DIR::LEFT:
				return "left";
				break;
			case ACTIVITY::DIR::STATIONARY:
				return "stationary";
				break;
			default:
				assert(0);
				return "";
				break;
		}
	}

	inline glm::ivec2 GETDIRECTION(ACTIVITY::DIR dir) {
		switch (dir) {
			case ACTIVITY::DIR::UP:
				return glm::ivec2(0, 1);
				break;
			case ACTIVITY::DIR::RIGHT:
				return glm::ivec2(1, 0);
				break;
			case ACTIVITY::DIR::DOWN:
				return glm::ivec2(0, -1);
				break;
			case ACTIVITY::DIR::LEFT:
				return glm::ivec2(-1, 0);
				break;
			case ACTIVITY::DIR::STATIONARY:
				return glm::ivec2(0, 0);
				break;
			default:
				assert(0);
				return glm::ivec2(0, 0);
				break;
		}
	}

	inline ACTIVITY::DIR FLIP(ACTIVITY::DIR dir) {
		switch (dir) {
			case ACTIVITY::DIR::UP:
				return ACTIVITY::DIR::DOWN;
				break;
			case ACTIVITY::DIR::RIGHT:
				return ACTIVITY::DIR::LEFT;
				break;
			case ACTIVITY::DIR::DOWN:
				return ACTIVITY::DIR::UP;
				break;
			case ACTIVITY::DIR::LEFT:
				return ACTIVITY::DIR::RIGHT;
				break;
			case ACTIVITY::DIR::STATIONARY:
				return ACTIVITY::DIR::STATIONARY;
				break;
			default:
				assert(0);
				return ACTIVITY::DIR::STATIONARY;
				break;
		}
	}
}

class Anchor;

class Activity
{
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
	ACTIVITY::DIR movementDirection = ACTIVITY::DIR::STATIONARY;
	bool moving = false;

	bool inWorld = false;

	glm::ivec2 origin;

public:
	WeakReference<Activity, GrouperBase> parentRef;
	Handle selfHandle;

	virtual glm::ivec2 getOrigin() {
		return origin;
	};
	Handle getHandle() {
		return selfHandle;
	};

	bool idleUp();
	virtual bool idleLocal();
	virtual bool moveableIdleLocal();
	virtual bool activityIdleLocal();

	float getMovementScale(int32_t  tick);
	float getActivityScale(int32_t  tick);

	// Constructors
	Activity() = default;
	Activity(Handle self, glm::ivec2 p) : selfHandle(self), origin(p), parentRef(0) {
	};
	virtual ~Activity() = default;

	// Placement 
	virtual void rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) = 0;
	void rotateForcedUp(glm::ivec2 center, ACTIVITY::ROT rotation);
	void forceMoveOriginUp(glm::ivec2 d);
	void forceMoveOriginLocal(glm::ivec2 d) {
		origin += d;
	};
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
	virtual bool canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) = 0;
	virtual void applyMoveLocalForced(GameState& gameState, ACTIVITY::DIR dir, int32_t pace);
	bool canMoveUp(GameState& gameState, ACTIVITY::DIR dir);
	bool canMoveUp(GameState& gameState, ACTIVITY::DIR dir, std::vector<Activity*>& extraIgnore);
	bool applyMoveUp(GameState& gameState, ACTIVITY::DIR dir, int32_t pace);
	void applyMoveUpForced(GameState& gameState, ACTIVITY::DIR dir, int32_t pace);
	bool applyMoveRoot(GameState& gameState, ACTIVITY::DIR dir, int32_t pace);
	void stopMovement(GameState& gameState);

	// Traces Placement
	virtual bool canFillTracesLocal(GameState& gameState) = 0;
	virtual void fillTracesLocalForced(GameState& gameState);
	virtual void removeTracesLocalForced(GameState& gameState);

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
	virtual ACTIVITY::TYPE getType() = 0;
	std::string getTypeName();
	virtual void save(Saver& saver);
	virtual bool load(Loader& loader);

	// Render
	virtual void appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) = 0;
	virtual void appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) = 0;
};

