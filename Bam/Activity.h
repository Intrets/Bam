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
		CLOCKWISE,
		COUNTERCLOCKWISE,
	} ROT;

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
		ACTIVATOR,
		RAILCRANE,
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
		"activator",
		"railcrane",
		"_MAX_SHOULD_NOT_SEE_THIS",
	};
}

class Anchor;

class Activity : public ModifyingInterface
{
private:
	Handle getRootHandle();

protected:
	glm::vec2 getMovingOrigin(GameState& gameState);

	int32_t activityPace = 10;
	int32_t activityTickStart = 0;
	int32_t activityType = 0;
	bool active = false;

	int32_t movingPace = 20;
	int32_t movingTickStart = 0;
	MOVEABLE::DIR movementDirection = MOVEABLE::STATIONARY;
	bool moving = false;

	glm::ivec2 origin;

public:
	WeakReference<Activity, Anchor> parentRef;
	Handle selfHandle;

	glm::ivec2 getOrigin() { return origin; };
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
	virtual void rotateForcedLocal(glm::ivec2 center, MOVEABLE::ROT rotation) = 0;
	void rotateForcedUp(glm::ivec2 center, MOVEABLE::ROT rotation);
	void forceMoveOriginUp(glm::ivec2 d);
	void forceMoveOriginLocal(glm::ivec2 d) { origin += d; };
	void disconnectFromParent();

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) = 0;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) = 0;
	bool applyActivityLocal(GameState& gameState, int32_t type, int32_t pace);
	void stopActivity(GameState& gameState);

	// Moveable
	virtual bool canMoveLocal(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) = 0;
	virtual void applyMoveLocalForced(GameState& gameState, MOVEABLE::DIR dir, int32_t pace);
	bool canMoveUp(GameState& gameState, MOVEABLE::DIR dir);
	bool canMoveUp(GameState& gameState, MOVEABLE::DIR dir, std::vector<Activity*>& extraIgnore);
	bool applyMoveUp(GameState& gameState, MOVEABLE::DIR dir, int32_t pace);
	void applyMoveUpForced(GameState& gameState, MOVEABLE::DIR dir, int32_t pace);
	bool applyMoveRoot(GameState& gameState, MOVEABLE::DIR dir, int32_t pace);
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
	WeakReference<Activity, Activity> getRoot();

	// Serial
	virtual ACTIVITY::TYPE getType() = 0;
	virtual void save(Saver& saver);
	virtual bool load(Loader& loader);

	// Render
	virtual void appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo, glm::vec4 color) = 0;
	virtual void appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) = 0;

	// Modifying
	virtual void fillModifyingMap(ModifyerBase& modifyer) override;
	using ModifyingInterface::getSimpleInfo;
	virtual std::ostream& getSimpleInfo(std::ostream& out) override;

	// TODO: move helper function
	glm::ivec2 getDirection(int32_t  i) {
		return MOVEABLE::DIRECTION[i];
	}
};

