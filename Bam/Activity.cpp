#include "common.h"
#include "Activity.h"
#include "GameState.h"
#include "Anchor.h"
#include "Modifyables.h"
#include "ActivityIgnoringGroup.h"
//#include "Saver.h"
//#include "Loader.h"

void Activity::doMove(GameState & gameState, MOVEABLE::DIR dir, int pace) {
	movingPace = pace;
	moving = true;
	movementDirection = dir;
	movingTickStart = gameState.tick;
	leaveMoveableTraces(gameState);
}

glm::vec2 Activity::getMovingOrigin(GameState& gameState) {
	int tick = gameState.tick;
	glm::vec2 v = glm::vec2(origin);
	if (moving) {
		float scale = static_cast<float>(tick - movingTickStart) / movingPace;
		v += scale * glm::vec2(getDirection(movementDirection));
	}
	return v;
}

bool Activity::idle() {
	return !moving && !active;
}

bool Activity::applyActivity(GameState & gameState, int useType, int pace) {
	if (moving) return false;
	if (!canActivity(gameState, useType, this)) return false;
	gameState.activityPaceHandler.add(WeakReference<Activity, Activity>(selfHandle), pace);
	doActivity(gameState, useType, pace);
	return true;
}

void Activity::doActivity(GameState & gameState, int useType, int pace) {
	doActivityInternal(gameState, useType, pace);
}

bool Activity::applyRootMove(GameState & gameState, MOVEABLE::DIR dir, int pace) {
	//auto r = ActivityWeakReference<Activity>(getRoot2());
	auto r = getRoot();
	//auto t = r.get();
	return r.get()->applyCurrentMove(gameState, dir, pace);
}

bool Activity::applyCurrentMove(GameState & gameState, MOVEABLE::DIR dir, int pace) {
	if (active) return false;
	// TODO
	ActivityIgnoringGroup group;
	getGroup(group);
	group.prepare();
	if (!canMove(gameState, dir, group)) return false;
	movingPace = pace;
	gameState.movementPaceHandler.add(WeakReference<Activity,Activity>(selfHandle), pace);
	doMove(gameState, dir, pace);
	return true;
}

void Activity::stopMovement(GameState& gameState) {
	origin += getDirection(movementDirection);
	removeMoveableTraces(gameState);
	movementDirection = MOVEABLE::STATIONARY;
	moving = false;
}

void Activity::getGroup(ActivityIgnoringGroup & ignore) {
	ignore.add(selfHandle);
}

WeakReference<Activity, Activity> Activity::getRoot() {
	return WeakReference<Activity, Activity>(getRootHandle());
}

Handle Activity::getRootHandle() {
	if (parentRef.isNotNull()) {
		return parentRef.get()->getRootHandle();
	}
	else {
		return selfHandle;
	}

}

void Activity::save(Saver & saver) {
	saver.store(getType());
	saver.store<int>(parentRef.handle);
	saver.store<int>(selfHandle);
	saver.store<int>(activityPace);
	saver.store(activityTickStart);
	saver.store<int>(activityType);
	saver.store<bool>(active);
	saver.store<int>(movingPace);
	saver.store<int>(movingTickStart);
	saver.store<MOVEABLE::DIR>(movementDirection);
	saver.store<bool>(moving);
	saver.store<glm::ivec2>(origin);
}

bool Activity::load(Loader & loader) {
	loader.retrieve<int>(parentRef.handle);
	loader.retrieve<int>(selfHandle);
	loader.retrieve<int>(activityPace);
	loader.retrieve(activityTickStart);
	loader.retrieve<int>(activityType);
	loader.retrieve<bool>(active);
	loader.retrieve<int>(movingPace);
	loader.retrieve<int>(movingTickStart);
	loader.retrieve<MOVEABLE::DIR>(movementDirection);
	loader.retrieve<bool>(moving);
	loader.retrieve<glm::ivec2>(origin);
	return true;
}

void Activity::fillModifyingMap(ModifyerBase & modifyer) {
	modifyer.modifyables["self"] = std::make_unique<ModifyableInt<Activity>>(&Activity::selfHandle);
	modifyer.modifyables["parent"] = std::make_unique<ModifyableAnchorRef<Activity>>(&Activity::parentRef);
	modifyer.modifyables["rootMove"] = std::make_unique<ModifyableDoRootMove<Activity>>(&Activity::applyRootMove);
	modifyer.modifyables["currentMove"] = std::make_unique<ModifyableDoRootMove<Activity>>(&Activity::applyCurrentMove);
	modifyer.modifyables["pos"] = std::make_unique<ModifyableIVec2<Activity>>(&Activity::origin);
	modifyer.modifyables["movePace"] = std::make_unique<ModifyableInt<Activity>>(&Activity::movingPace);
	modifyer.modifyables["doActivity"] = std::make_unique<ModifyableDoActivity<Activity>>(&Activity::applyActivity);
}

std::ostream & Activity::getSimpleInfo(std::ostream & out) {
	std::string typeName = ACTIVITY::TYPE_NAMES[getType()];
	out << typeName << ": " << selfHandle;
	return out;
}

void Activity::stopActivity(GameState & gameState) {
	removeActivityTraces(gameState);
	activityType = 0;
	active = false;
}
