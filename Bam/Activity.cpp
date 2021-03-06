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
	gameState.movementPaceHandler.add(this->getSelfReference(), pace);
	this->postMoveableStartLocal(gameState);
}

bool Activity::canMoveUp(GameState& gameState, ACTIVITY::DIR dir) {
	ActivityIgnoringGroup ignoring(this->getSortedMembers());
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

WeakReference<Activity, Activity> Activity::getSelfReference() {
	return this->selfReference;
}

glm::vec2 Activity::getMovingOrigin(GameState const& gameState) const {
	glm::vec2 v = glm::vec2(this->origin);
	if (this->moving) {
		v += this->getMovementScaleForced(gameState.tick) * glm::vec2(GETDIRECTION(this->movementDirection));
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

bool Activity::isInWorld() {
	return this->inWorld;
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

float Activity::getMovementScale(int32_t tick) const {
	if (this->moving) {
		return this->getMovementScaleForced(tick);
	}
	else {
		return 0.0f;
	}
}

float Activity::getActivityScale(int32_t tick) const {
	if (this->active) {
		return this->getActivityScaleForced(tick);
	}
	else {
		return 0.0f;
	}
}

float Activity::getMovementScaleForced(int32_t tick) const {
	return static_cast<float>(1 + tick - this->movingTickStart) / (this->movingPace + 1);
}

float Activity::getActivityScaleForced(int32_t tick) const {
	return static_cast<float>(1 + tick - this->activityTickStart) / (this->activityPace + 1);
}

void Activity::rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) {
	this->activityRotation = ACTIVITY::ROTATE(rotation, this->activityRotation);
	this->baseBlock.rotate(rotation);
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

			if (auto removed = anchor->removeChild(this->getSelfReference())) {
				removed.value().clear();

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
			if (auto removed = this->parentRef.get()->removeChild(this->getSelfReference())) {
				removed.value().clear();
			}
			else {
				assert(0);
			}
		}

		this->parentRef.clear();
	}
}

void Activity::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	this->activityPace = pace;
	this->active = true;
	this->activityTickStart = gameState.tick;
	this->activityType = type;
	gameState.activityPaceHandler.add(this->getSelfReference(), pace);
}

bool Activity::applyActivityLocal(GameState& gameState, int32_t useType) {
	if (!this->activityIdleLocal()) {
		return false;
	}
	if (!this->canActivityLocal(gameState, useType)) {
		return false;
	}
	this->applyActivityLocalForced(gameState, useType, 10);
	return true;
}

bool Activity::applyMoveUp(GameState& gameState, ACTIVITY::DIR dir, int32_t pace) {
	auto const& members = this->getTreeMembers();

	for (auto member : members) {
		if (!member->moveableIdleLocal()) {
			return false;
		}
	}

	ActivityIgnoringGroup ignoring(this->getSortedMembers());
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
	this->preMoveableStopLocal(gameState);
	this->movementDirection = ACTIVITY::DIR::STATIONARY;
	this->moving = false;
}

std::vector<Activity*> const& Activity::getSortedMembers() {
	return this->memberCache.getSortedMembers();
}

std::vector<Activity*> const& Activity::getTreeMembers() {
	return this->memberCache.getMembers();
}

WeakReference<Activity, Activity> Activity::getRootRef() {
	return this->memberCache.getRoot()->getSelfReference();
}

Handle Activity::getRootHandle() {
	return this->memberCache.getRoot()->getHandle();
}

Activity* Activity::getRootPtr() {
	return this->memberCache.getRoot();
}

Activity* Activity::impl_getRoot() {
	if (this->parentRef.isNotNull()) {
		return this->parentRef.get()->impl_getRoot();
	}
	else {
		return this;
	}
}

std::string Activity::getTypeName() {
	return ACTIVITY::TYPE_NAMES[static_cast<int32_t>(this->getType())];
}

void Activity::save(Saver& saver) {
	saver.store(this->getType());
	saver.store(this->parentRef);
	saver.store(this->selfHandle);
	saver.store(this->selfReference);
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
	this->baseBlock.save(saver);
}

bool Activity::load(Loader& loader) {
	loader.retrieve(this->parentRef);
	loader.retrieve(this->selfHandle);
	loader.retrieve(this->selfReference);
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
	this->baseBlock.load(loader);
	return true;
}

void Activity::stopActivity(GameState& gameState) {
	this->preActivityStopLocal(gameState);
	this->activityType = 0;
	this->active = false;
	this->postActivityStopLocal(gameState);
}

void Activity::forceChangeActivityState(int32_t type) {
}

bool Activity::canFillTracesUp(GameState& gameState) {
	auto const& members = this->getTreeMembers();
	for (auto member : members) {
		if (!member->canFillTracesLocal(gameState)) {
			return false;
		}
	}
	return true;
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

void Activity::postActivityStopLocal(GameState& gameState) {
}

void Activity::preActivityStopLocal(GameState& gameState) {
}

