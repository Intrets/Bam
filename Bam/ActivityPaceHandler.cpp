#include "common.h"
#include "ActivityPaceHandler.h"
#include "GameState.h"

#include "Activity.h"
//#include "Saver.h"
//#include "Loader.h"

ActivityPaceHandler::ActivityPaceHandler(int32_t max) : activities(max) {
}

ActivityPaceHandler::ActivityPaceHandler() : ActivityPaceHandler(600) {
}

void ActivityPaceHandler::finish(GameState& gameState) {
	for (auto& a : activities[index]) {
		a.get()->stopActivity(gameState);
	}
}

void ActivityPaceHandler::cycle() {
	activities[index].clear();
	index++;
	index %= activities.size();
}

void ActivityPaceHandler::add(WeakReference<Activity, Activity> m, int32_t duration) {
	activities[(index + duration) % activities.size()].push_back(m);
}

void ActivityPaceHandler::save(Saver& saver) {
	saver.store(index);
	saver.store(activities.size());
	for (auto& v : activities) {
		saver.store(v.size());
		for (auto& ref : v) {
			saver.store(ref);
		}
	}
}

void ActivityPaceHandler::load(Loader& loader) {
	loader.retrieve(index);
	size_t size;
	loader.retrieve(size);
	activities.clear();
	activities.resize(size);
	for (int32_t i = 0; i < size; i++) {
		size_t size2;
		loader.retrieve(size2);
		for (int32_t j = 0; j < size2; j++) {
			WeakReference<Activity, Activity> ref;
			loader.retrieve(ref);
			activities[i].push_back(ref);
		}
	}
}

MovementPaceHandler::MovementPaceHandler(int32_t max) : activities(max) {
}

MovementPaceHandler::MovementPaceHandler() : MovementPaceHandler(600) {
}

void MovementPaceHandler::finish(GameState& gameState) {
	for (auto& a : activities[index]) {
		a.get()->stopMovement(gameState);
	}
}

void MovementPaceHandler::save(Saver& saver) {
	saver.store(index);
	saver.store(activities.size());
	for (auto& v : activities) {
		saver.store(v.size());
		for (auto& ref : v) {
			saver.store(ref.handle);
		}
	}
}

void MovementPaceHandler::load(Loader& loader) {
	loader.retrieve(index);
	size_t size;
	loader.retrieve(size);
	activities.clear();
	activities.resize(size);
	for (size_t i = 0; i < size; i++) {
		size_t size2;
		loader.retrieve(size2);
		for (size_t j = 0; j < size2; j++) {
			WeakReference<Activity, Activity> ref;
			loader.retrieve(ref);
			activities[i].push_back(ref);
		}
	}
}

void MovementPaceHandler::cycle() {
	activities[index].clear();
	index++;
	index %= activities.size();
}

void MovementPaceHandler::add(WeakReference<Activity, Activity> m, int32_t duration) {
	activities[(index + duration) % activities.size()].push_back(m);
}
