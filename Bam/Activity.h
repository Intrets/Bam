#pragma once

#include "ReferenceManager.h"

#include <initializer_list>
#include "MemberCache.h"
#include "ActivityCopier.h"

class ActivityIgnoringGroup;
class GrouperBase;
class Loader;
class Saver;
class GameState;
struct StaticWorldRenderInfo;
struct RenderInfo;

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
		READER,
		DETECTOR,
		INCINERATOR,
		FORWARDER,
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
		"reader",
		"detector",
		"incinerator",
		"forwarder",
		"_MAX_SHOULD_NOT_SEE_THIS",
	};

	enum DIR
	{
		RIGHT,
		DOWN,
		LEFT,
		UP,
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

	inline ACTIVITY::DIR CLOCKWISEROT_WITH_STATIONARY(ACTIVITY::DIR dir) {
		return static_cast<ACTIVITY::DIR>((static_cast<int32_t>(dir) + 1) % 5);
	}

	inline ACTIVITY::DIR COUNTERWISEROT(ACTIVITY::DIR dir) {
		return static_cast<ACTIVITY::DIR>((static_cast<int32_t>(dir) + 3) % 4);
	}

	inline ACTIVITY::DIR COUNTERWISEROT_WITH_STATIONARY(ACTIVITY::DIR dir) {
		return static_cast<ACTIVITY::DIR>((static_cast<int32_t>(dir) + 4) % 5);
	}

	inline ACTIVITY::DIR ROTATE(ACTIVITY::ROT rot, ACTIVITY::DIR dir) {
		if (rot == ACTIVITY::ROT::CLOCKWISE) {
			return CLOCKWISEROT(dir);
		}
		else {
			return COUNTERWISEROT(dir);
		}
	}

	inline ACTIVITY::DIR ROTATE_WITH_STATIONARY(ACTIVITY::ROT rot, ACTIVITY::DIR dir) {
		if (rot == ACTIVITY::ROT::CLOCKWISE) {
			return CLOCKWISEROT_WITH_STATIONARY(dir);
		}
		else {
			return COUNTERWISEROT_WITH_STATIONARY(dir);
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
	// non-cached base implementation
	Handle impl_getRootHandle() const;

	friend class MemberCache;

protected:
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

	ACTIVITY::DIR activityRotation = ACTIVITY::DIR::RIGHT;

	std::string label;

	friend class ACTIVITYCOPIER;

public:
	MemberCache memberCache{ *this };

	WeakReference<Activity, GrouperBase> parentRef;

	// Get's set by non default constructor, and should be set after default constructor
	Handle selfHandle = -1;

	glm::vec2 getMovingOrigin(GameState const& gameState) const;

	virtual glm::ivec2 getOrigin() {
		return origin;
	};
	Handle getHandle() {
		return selfHandle;
	};

	std::string const& getLabel() const;
	void setLabel(std::string text);

	bool idleUp();
	virtual bool idleLocal();
	virtual bool moveableIdleLocal();
	virtual bool activityIdleLocal();

	// With check if activity is moving
	float getMovementScale(int32_t tick) const;
	// With check if activity is active
	float getActivityScale(int32_t tick) const;

	// Without check if activity is moving
	float getMovementScaleForced(int32_t tick) const;
	// Without check if activity is active
	float getActivityScaleForced(int32_t tick) const;

	// Invalid state, needs to set member variables selfHandle, origin and parentRef
	Activity() = default;
	Activity(Handle self, glm::ivec2 p) : selfHandle(self), origin(p), parentRef(0) {
	};
	virtual ~Activity() = default;

	// Placement 
	virtual void rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation);
	void rotateForcedUp(glm::ivec2 center, ACTIVITY::ROT rotation);
	void forceMoveOriginUp(glm::ivec2 d);
	void forceMoveOriginLocal(glm::ivec2 d) {
		origin += d;
	};
	void disconnectFromParent();

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) = 0;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) = 0;
	bool applyActivityLocal(GameState& gameState, int32_t type);
	void stopActivity(GameState& gameState);

	// Forcefully change the activity state, only use when activity is not in static world.
	// The type is e.g. retracting or extending a piston
	virtual void forceChangeActivityState(int32_t type);

	// Moveable
	virtual bool canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) = 0;
	virtual void applyMoveLocalForced(GameState& gameState, ACTIVITY::DIR dir, int32_t pace);
	bool canMoveUp(GameState& gameState, ACTIVITY::DIR dir);
	bool canMoveUp(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore);
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
	virtual void postActivityStopLocal(GameState& gameState);
	virtual void preActivityStopLocal(GameState& gameState);

	// Traces Moveable
	virtual void preMoveableStopLocal(GameState& gameState) = 0;
	virtual void postMoveableStartLocal(GameState& gameState) = 0;

	// Tree Informations

	// non-cached base implementation
	virtual void impl_getTreeMembers(std::vector<Activity*>& members) = 0;
	// non-cached base implementation
	virtual void impl_getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) = 0;

	// cached through MemberCache member object
	std::vector<Handle> const& getSortedHandles();
	// cached through MemberCache member object
	std::vector<Activity*> const& getTreeMembers();
	// cached through MemberCache member object
	WeakReference<Activity, Activity> getRootRef();
	// cached through MemberCache member object
	Handle getRootHandle();
	// cached through MemberCache member object
	Activity* getRootPtr();

	// Serial
	virtual ACTIVITY::TYPE getType() = 0;
	std::string getTypeName();
	virtual void save(Saver& saver);
	virtual bool load(Loader& loader);

	// Render
	virtual void appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) = 0;
	virtual void appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) = 0;
};

