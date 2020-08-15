#include "common.h"
#include "Activity.h"
#include "GameState.h"
#include "Anchor.h"
#include "ActivityIgnoringGroup.h"
#include "Saver.h"
#include "Loader.h"

void Activity::applyMoveLocalForced(GameState& gameState, Activity::DIR dir, int32_t pace) {
	this->movingPace = pace;
	this->moving = true;
	this->movementDirection = dir;
	this->movingTickStart = gameState.tick;
	gameState.movementPaceHandler.add(WeakReference<Activity, Activity>(this->selfHandle), pace);
	this->leaveMoveableTracesLocal(gameState);
}

bool Activity::canMoveUp(GameState& gameState, Activity::DIR dir) {
	std::vector<Activity*> members;
	this->getTreeMembers(members);

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

bool Activity::canMoveUp(GameState& gameState, Activity::DIR dir, std::vector<Activity*>& extraIgnore) {
	std::vector<Activity*> members;
	this->getTreeMembers(members);

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
	int32_t tick = gameState.tick;
	glm::vec2 v = glm::vec2(this->origin);
	if (this->moving) {
		float scale = static_cast<float>(tick - this->movingTickStart) / this->movingPace;
		v += scale * glm::vec2(getDirection(this->movementDirection));
	}
	return v;
}

bool Activity::idleUp() {
	std::vector<Activity*> members;
	this->getTreeMembers(members);
	for (auto& member : members) {
		if (!member->idleLocal()) {
			return false;
		}
	}
	return true;
}

bool Activity::idleLocal() {
	return !this->moving && !this->active;
}

bool Activity::moveableIdleLocal() {
	return this->idleLocal();
}

bool Activity::activityIdleLocal() {
	return this->idleLocal();
}

float Activity::getMovementScale(int32_t tick) {
	if (this->moving) {
		return static_cast<float>(tick - this->movingTickStart) / this->movingPace;
	}
	else {
		return 0.0f;
	}
}

float Activity::getActivityScale(int32_t tick) {
	if (this->active) {
		return static_cast<float>(tick - this->activityTickStart) / this->activityPace;
	}
	else {
		return 0.0f;
	}
}

void Activity::rotateForcedUp(glm::ivec2 center, Activity::ROT rotation) {
	std::vector<Activity*> members;
	this->getTreeMembers(members);
	for (auto member : members) {
		member->rotateForcedLocal(center, rotation);
	}
}

void Activity::forceMoveOriginUp(glm::ivec2 d) {
	std::vector<Activity*> members;
	this->getTreeMembers(members);
	for (auto& member : members) {
		member->forceMoveOriginLocal(d);
	}
}

void Activity::disconnectFromParent() {
	if (this->parentRef.isNotNull()) {
		auto anchor = this->parentRef.get();
		if (anchor->removeChild(this->selfHandle)) {
			anchor->disconnectFromParent();
			parentRef.deleteObject();
		}
		parentRef.handle = 0;
	}
}

void Activity::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	this->activityPace = pace;
	this->active = true;
	this->activityTickStart = gameState.tick;
	gameState.activityPaceHandler.add(WeakReference<Activity, Activity>(this->selfHandle), pace);
}

bool Activity::applyActivityLocal(GameState& gameState, int32_t useType, int32_t pace) {
	if (!this->activityIdleLocal()) {
		return false;
	}
	if (!this->canActivityLocal(gameState, useType)) {
		return false;
	}
	this->applyActivityLocalForced(gameState, useType, pace);
	return true;
}

bool Activity::applyMoveUp(GameState& gameState, Activity::DIR dir, int32_t pace) {
	std::vector<Activity*> members;
	this->getTreeMembers(members);

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

void Activity::applyMoveUpForced(GameState& gameState, Activity::DIR dir, int32_t pace) {
	std::vector<Activity*> members;
	this->getTreeMembers(members);

	for (auto member : members) {
		member->applyMoveLocalForced(gameState, dir, pace);
	}
}

bool Activity::applyMoveRoot(GameState& gameState, Activity::DIR dir, int32_t pace) {
	return this->getRoot().get()->applyMoveUp(gameState, dir, pace);
}

void Activity::stopMovement(GameState& gameState) {
	this->origin += this->getDirection(this->movementDirection);
	this->removeMoveableTracesLocal(gameState);
	this->movementDirection = Activity::DIR::STATIONARY;
	this->moving = false;
}

WeakReference<Activity, Activity> Activity::getRoot() {
	return WeakReference<Activity, Activity>(this->getRootHandle());
}

Handle Activity::getRootHandle() {
	if (this->parentRef.isNotNull()) {
		return this->parentRef.get()->getRootHandle();
	}
	else {
		return this->selfHandle;
	}

}

void Activity::save(Saver& saver) {
	saver.store(this->getType());
	saver.store(this->parentRef.handle);
	saver.store(this->selfHandle);
	saver.store(this->activityPace);
	saver.store(this->activityTickStart);
	saver.store(this->activityType);
	saver.store(this->active);
	saver.store(this->movingPace);
	saver.store(this->movingTickStart);
	saver.store(this->movementDirection);
	saver.store(this->moving);
	saver.store(this->origin);
}

bool Activity::load(Loader& loader) {
	loader.retrieve(this->parentRef.handle);
	loader.retrieve(this->selfHandle);
	loader.retrieve(this->activityPace);
	loader.retrieve(this->activityTickStart);
	loader.retrieve(this->activityType);
	loader.retrieve(this->active);
	loader.retrieve(this->movingPace);
	loader.retrieve(this->movingTickStart);
	loader.retrieve(this->movementDirection);
	loader.retrieve(this->moving);
	loader.retrieve(this->origin);
	return true;
}

void Activity::appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo, glm::vec4 color) {
}

void Activity::stopActivity(GameState& gameState) {
	this->activityType = 0;
	this->active = false;
	this->removeActivityTracesLocal(gameState);
}

void Activity::fillTracesUpForced(GameState& gameState) {
	std::vector<Activity*> members;
	this->getTreeMembers(members);
	for (auto member : members) {
		member->fillTracesLocalForced(gameState);
	}
}

bool Activity::fillTracesUp(GameState& gameState) {
	std::vector<Activity*> members;
	this->getTreeMembers(members);
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
	this->getTreeMembers(members);
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
