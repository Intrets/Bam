#include "common.h"
#include "Activity.h"
#include "GameState.h"
#include "Anchor.h"
#include "Modifyables.h"
#include "ActivityIgnoringGroup.h"
//#include "Saver.h"
//#include "Loader.h"

void Activity::applyMoveLocalForced(GameState& gameState, MOVEABLE::DIR dir, int pace) {
	movingPace = pace;
	moving = true;
	movementDirection = dir;
	movingTickStart = gameState.tick;
	gameState.movementPaceHandler.add(WeakReference<Activity, Activity>(selfHandle), pace);
	leaveMoveableTracesLocal(gameState);
}

bool Activity::canMoveUp(GameState& gameState, MOVEABLE::DIR dir) {
	std::vector<Activity*> members;
	getTreeMembers(members);

	for (auto member : members) {
		if (!member->moveableIdleLocal()) {
			return false;
		}
	}

	ActivityIgnoringGroup ignoring(members);
	for (auto member : members) {
		if (!member->idleLocal() || !member->canMoveLocal(gameState, dir, ignoring)) {
			return false;
		}
	}
	return true;
}

bool Activity::canMoveUp(GameState& gameState, MOVEABLE::DIR dir, std::vector<Activity*>& extraIgnore) {
	std::vector<Activity*> members;
	getTreeMembers(members);

	for (auto member : members) {
		if (!member->moveableIdleLocal()) {
			return false;
		}
	}

	ActivityIgnoringGroup ignoring(members, extraIgnore);
	for (auto member : members) {
		if (!member->idleLocal() || !member->canMoveLocal(gameState, dir, ignoring)) {
			return false;
		}
	}
	return true;
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

bool Activity::idleLocal() {
	return !moving && !active;
}

bool Activity::moveableIdleLocal() {
	return idleLocal();
}

bool Activity::activityIdleLocal() {
	return idleLocal();
}

void Activity::rotateForcedUp(glm::ivec2 center, MOVEABLE::ROT rotation) {
	std::vector<Activity*> members;
	getTreeMembers(members);
	for (auto member : members) {
		member->rotateForcedLocal(center, rotation);
	}
}

void Activity::disconnectFromParent() {
	if (parentRef.isNotNull()) {
		auto anchor = parentRef.get();
		anchor->removeChild(selfHandle);
	}
}

void Activity::applyActivityLocalForced(GameState& gameState, int type, int pace) {
	activityPace = pace;
	active = true;
	activityTickStart = gameState.tick;
	gameState.activityPaceHandler.add(WeakReference<Activity, Activity>(selfHandle), pace);
}

bool Activity::applyActivityLocal(GameState& gameState, int useType, int pace) {
	if (!activityIdleLocal()) {
		return false;
	}
	if (!canActivityLocal(gameState, useType)) {
		return false;
	}
	applyActivityLocalForced(gameState, useType, pace);
	return true;
}

bool Activity::applyMoveUp(GameState& gameState, MOVEABLE::DIR dir, int pace) {
	std::vector<Activity*> members;
	getTreeMembers(members);

	for (auto member : members) {
		if (!member->moveableIdleLocal()) {
			return false;
		}
	}

	ActivityIgnoringGroup ignoring(members);
	for (auto member : members) {
		if (!member->canMoveLocal(gameState, dir, ignoring)) {
			return false;
		}
	}

	for (auto member : members) {
		member->applyMoveLocalForced(gameState, dir, pace);
	}
	return true;
}

void Activity::applyMoveUpForced(GameState& gameState, MOVEABLE::DIR dir, int pace) {
	std::vector<Activity*> members;
	getTreeMembers(members);

	for (auto member : members) {
		member->applyMoveLocalForced(gameState, dir, pace);
	}
}

bool Activity::applyMoveRoot(GameState& gameState, MOVEABLE::DIR dir, int pace) {
	return getRoot().get()->applyMoveUp(gameState, dir, pace);
}

void Activity::stopMovement(GameState& gameState) {
	origin += getDirection(movementDirection);
	removeMoveableTracesLocal(gameState);
	movementDirection = MOVEABLE::STATIONARY;
	moving = false;
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

void Activity::save(Saver& saver) {
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

bool Activity::load(Loader& loader) {
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

void Activity::fillModifyingMap(ModifyerBase& modifyer) {
	modifyer.modifyables["self"] = std::make_unique<ModifyableInt<Activity>>(&Activity::selfHandle);
	modifyer.modifyables["parent"] = std::make_unique<ModifyableAnchorRef<Activity>>(&Activity::parentRef);
	modifyer.modifyables["rootMove"] = std::make_unique<ModifyableDoRootMove<Activity>>(&Activity::applyMoveUp);
	//modifyer.modifyables["currentMove"] = std::make_unique<ModifyableDoRootMove<Activity>>(&Activity::applyMoveOLD);
	modifyer.modifyables["pos"] = std::make_unique<ModifyableIVec2<Activity>>(&Activity::origin);
	modifyer.modifyables["movePace"] = std::make_unique<ModifyableInt<Activity>>(&Activity::movingPace);
	modifyer.modifyables["applyActivityLocalForced"] = std::make_unique<ModifyableDoActivity<Activity>>(&Activity::applyActivityLocal);
}

std::ostream& Activity::getSimpleInfo(std::ostream& out) {
	std::string typeName = ACTIVITY::TYPE_NAMES[getType()];
	out << typeName << ": " << selfHandle;
	return out;
}

void Activity::stopActivity(GameState& gameState) {
	activityType = 0;
	active = false;
	removeActivityTracesLocal(gameState);
}

void Activity::fillTracesUpForced(GameState& gameState) {
	std::vector<Activity*> members;
	getTreeMembers(members);
	for (auto member : members) {
		member->fillTracesLocalForced(gameState);
	}
}

bool Activity::fillTracesUp(GameState& gameState) {
	std::vector<Activity*> members;
	getTreeMembers(members);
	for (auto member : members) {
		if (!member->canFillTracesLocal(gameState)) {
			return false;
		}
	}

	for (auto member : members) {
		member->fillTracesLocalForced(gameState);
	}
	return true;
}

void Activity::removeTracesUpForced(GameState& gameState) {
}

bool Activity::removeTracesUp(GameState& gameState) {
	std::vector<Activity*> members;
	getTreeMembers(members);
	for (auto member : members) {
		if (!member->idleLocal()) {
			return false;
		}
	}

	for (auto member : members) {
		member->removeTracesLocalForced(gameState);
	}
	return true;
}
