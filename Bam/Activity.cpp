#include "common.h"
#include "Activity.h"
#include "GameState.h"
#include "Grouper.h"
#include "ActivityIgnoringGroup.h"
#include "Saver.h"
#include "Loader.h"

void Activity::applyMoveLocalForced(GameState& gameState, ACTIVITY::DIR dir, int32_t pace) {
	this->movingPace = pace;
	this->moving = true;
	this->movementDirection = dir;
	this->movingTickStart = gameState.tick;
	gameState.movementPaceHandler.add(WeakReference<Activity, Activity>(this->selfHandle), pace);
	this->leaveMoveableTracesLocal(gameState);
}

bool Activity::canMoveUp(GameState& gameState, ACTIVITY::DIR dir) {
	ActivityIgnoringGroup ignoring(this->getSortedHandles());
	return this->canMoveUp(gameState, dir, ignoring);
}

bool Activity::canMoveUp(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignoring) {
	auto const& members = this->getTreeMembers();

	for (auto member : members) {
		if (!member->moveableIdleLocal()) {
			return false;
		}
	}

	for (auto member : members) {
		if (!member->idleLocal() || !member->canMoveLocal(gameState, dir, ignoring)) {
			return false;
		}
	}
	return true;
}

glm::vec2 Activity::getMovingOrigin(GameState const& gameState) const {
	int32_t tick = gameState.tick;
	glm::vec2 v = glm::vec2(this->origin);
	if (this->moving) {
		float scale = static_cast<float>(tick - this->movingTickStart) / this->movingPace;
		v += scale * glm::vec2(GETDIRECTION(this->movementDirection));
	}
	return v;
}

std::string const& Activity::getLabel() const {
	return this->label;
}

void Activity::setLabel(std::string text) {
	this->label = text;
	for (auto member : this->getRootPtr()->getTreeMembers()) {
		member->memberCache.invalidateMembers();
	}
}

bool Activity::idleUp() {
	for (auto& member : this->getTreeMembers()) {
		if (!member->idleLocal()) {
			return false;
		}
	}
	return true;
}

bool Activity::idleLocal() {
	return !this->moving && !this->active && this->inWorld;
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

void Activity::rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) {
	this->activityRotation = ACTIVITY::ROTATE(rotation, this->activityRotation);
}

void Activity::rotateForcedUp(glm::ivec2 center, ACTIVITY::ROT rotation) {
	for (auto member : this->getTreeMembers()) {
		member->rotateForcedLocal(center, rotation);
	}
}

void Activity::forceMoveOriginUp(glm::ivec2 d) {
	for (auto& member : this->getTreeMembers()) {
		member->forceMoveOriginLocal(d);
	}
}

void Activity::disconnectFromParent() {
	if (this->parentRef.isNotNull()) {
		if (this->parentRef.get()->getType() == ACTIVITY::TYPE::ANCHOR) {
			auto anchor = this->parentRef.get();

			if (auto removed = anchor->removeChild(this->selfHandle)) {
				removed.value().handle = 0;

				if (!anchor->hasChild()) {
					anchor->disconnectFromParent();
					parentRef.deleteObject();
				}
			}
			else {
				assert(0);
			}
		}
		else {
			if (auto removed = this->parentRef.get()->removeChild(this->getHandle())) {
				removed.value().handle = 0;
			}
			else {
				assert(0);
			}
		}

		this->parentRef.handle = 0;
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

bool Activity::applyMoveUp(GameState& gameState, ACTIVITY::DIR dir, int32_t pace) {
	auto const& members = this->getTreeMembers();

	for (auto member : members) {
		if (!member->moveableIdleLocal()) {
			return false;
		}
	}

	ActivityIgnoringGroup ignoring(this->getSortedHandles());
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

void Activity::applyMoveUpForced(GameState& gameState, ACTIVITY::DIR dir, int32_t pace) {
	for (auto member : this->getTreeMembers()) {
		member->applyMoveLocalForced(gameState, dir, pace);
	}
}

bool Activity::applyMoveRoot(GameState& gameState, ACTIVITY::DIR dir, int32_t pace) {
	return this->getRootPtr()->applyMoveUp(gameState, dir, pace);
}

void Activity::stopMovement(GameState& gameState) {
	this->origin += ACTIVITY::GETDIRECTION(this->movementDirection);
	this->removeMoveableTracesLocal(gameState);
	this->movementDirection = ACTIVITY::DIR::STATIONARY;
	this->moving = false;
}

std::vector<Handle> const& Activity::getSortedHandles() {
	return this->memberCache.getSortedHandles();
}

std::vector<Activity*> const& Activity::getTreeMembers() {
	return this->memberCache.getMembers();
}

WeakReference<Activity, Activity> Activity::getRootRef() {
	return WeakReference<Activity, Activity>(this->memberCache.getRoot()->getHandle());
}

Handle Activity::getRootHandle() {
	return this->memberCache.getRoot()->getHandle();
}

Activity* Activity::getRootPtr() {
	return this->memberCache.getRoot();
}

Handle Activity::impl_getRootHandle() const {
	if (this->parentRef.isNotNull()) {
		return this->parentRef.get()->impl_getRootHandle();
	}
	else {
		return this->selfHandle;
	}

}

std::string Activity::getTypeName() {
	return ACTIVITY::TYPE_NAMES[static_cast<int32_t>(this->getType())];
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
	saver.store(this->inWorld);
	saver.store(this->label);
	saver.store(this->activityRotation);
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
	loader.retrieve(this->inWorld);
	loader.retrieve(this->label);
	loader.retrieve(this->activityRotation);
	return true;
}

void Activity::stopActivity(GameState& gameState) {
	this->activityType = 0;
	this->active = false;
	this->removeActivityTracesLocal(gameState);
}

void Activity::forceChangeActivityState(int32_t type) {
}

void Activity::fillTracesLocalForced(GameState& gameState) {
	this->inWorld = true;
}

void Activity::removeTracesLocalForced(GameState& gameState) {
	this->inWorld = false;
}

void Activity::fillTracesUpForced(GameState& gameState) {
	for (auto member : this->getTreeMembers()) {
		member->fillTracesLocalForced(gameState);
		member->inWorld = true;
	}
}

bool Activity::fillTracesUp(GameState& gameState) {
	auto const& members = this->getTreeMembers();
	for (auto member : members) {
		if (!member->canFillTracesLocal(gameState)) {
			return false;
		}
	}

	for (auto member : members) {
		member->fillTracesLocalForced(gameState);
		member->inWorld = true;
	}
	return true;
}

void Activity::removeTracesUpForced(GameState& gameState) {
	for (auto member : this->getTreeMembers()) {
		member->removeTracesLocalForced(gameState);
		member->inWorld = false;
	}
}

bool Activity::removeTracesUp(GameState& gameState) {
	auto const& members = this->getTreeMembers();
	for (auto member : members) {
		if (!member->idleLocal()) {
			return false;
		}
	}

	for (auto member : members) {
		member->removeTracesLocalForced(gameState);
		member->inWorld = false;
	}
	return true;
}

