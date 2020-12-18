#include "common.h"

#include "ActivityPaceHandler.h"
#include "Activity.h"

#include "GameState.h"
#include "Saver.h"
#include "Loader.h"

ActivityPaceHandler::ActivityPaceHandler(int32_t max) : activities(max) {
}

ActivityPaceHandler::ActivityPaceHandler() : ActivityPaceHandler(600) {
}

void ActivityPaceHandler::finish(GameState& gameState) {
	for (auto& a : this->activities[this->index]) {
		a.get()->stopActivity(gameState);
	}
}

void ActivityPaceHandler::cycle() {
	this->activities[this->index].clear();
	this->index++;
	this->index %= this->activities.size();
}

void ActivityPaceHandler::add(WeakReference<Activity, Activity> m, int32_t duration) {
	this->activities[(this->index + duration) % this->activities.size()].push_back(m);
}

void ActivityPaceHandler::save(Saver& saver) {
	saver.store(this->index);
	saver.store(this->activities.size());
	for (auto& v : this->activities) {
		saver.store(v.size());
		for (auto& ref : v) {
			saver.store(ref);
		}
	}
}

void ActivityPaceHandler::load(Loader& loader) {
	loader.retrieve(this->index);
	size_t size;
	loader.retrieve(size);
	this->activities.clear();
	this->activities.resize(size);
	for (int32_t i = 0; i < size; i++) {
		size_t size2;
		loader.retrieve(size2);
		for (int32_t j = 0; j < size2; j++) {
			WeakReference<Activity, Activity> ref;
			loader.retrieve(ref);
			this->activities[i].push_back(ref);
		}
	}
}

MovementPaceHandler::MovementPaceHandler(int32_t max) : activities(max) {
}

MovementPaceHandler::MovementPaceHandler() : MovementPaceHandler(600) {
}

void MovementPaceHandler::finish(GameState& gameState) {
	for (auto& a : this->activities[this->index]) {
		a.get()->stopMovement(gameState);
	}
}

void MovementPaceHandler::save(Saver& saver) {
	saver.store(this->index);
	saver.store(this->activities.size());
	for (auto& v : activities) {
		saver.store(v.size());
		for (auto& ref : v) {
			saver.store(ref);
		}
	}
}

void MovementPaceHandler::load(Loader& loader) {
	loader.retrieve(this->index);
	size_t size;
	loader.retrieve(size);
	this->activities.clear();
	this->activities.resize(size);
	for (size_t i = 0; i < size; i++) {
		size_t size2;
		loader.retrieve(size2);
		this->activities[i].resize(size2);
		for (size_t j = 0; j < size2; j++) {
			loader.retrieve(this->activities[i][j]);
		}
	}
}

void MovementPaceHandler::cycle() {
	this->activities[this->index].clear();
	this->index++;
	this->index %= this->activities.size();
}

void MovementPaceHandler::add(WeakReference<Activity, Activity> m, int32_t duration) {
	this->activities[(this->index + duration) % this->activities.size()].push_back(m);
}
