#include "common.h"
#include "ActivityPaceHandler.h"
#include "GameState.h"

#include "Activity.h"
//#include "Saver.h"
//#include "Loader.h"

ActivityPaceHandler::ActivityPaceHandler(int32_t max) : activities(max), size(max) {
}

ActivityPaceHandler::ActivityPaceHandler() : ActivityPaceHandler(600) {
}

void ActivityPaceHandler::finish(GameState& gameState) {
	for (auto& a : activities[i]) {
		a.get()->stopActivity(gameState);
	}
}

void ActivityPaceHandler::cycle() {
	activities[i].clear();
	i++;
	i %= size;
}

void ActivityPaceHandler::add(WeakReference<Activity, Activity> m, int32_t duration) {
	activities[(i + duration) % size].push_back(m);
}

void ActivityPaceHandler::save(Saver & saver) {
	//saver.store(i);
	//saver.store(size);
	//for (auto& v : activities) {
	//	saver.store<int>(v.size());
	//	for (auto& ref : v) {
	//		saver.store(ref.handle);
	//	}
	//}
}

void ActivityPaceHandler::load(Loader & loader) {
	//loader.retrieve(i);
	//loader.retrieve(size);
	//activities.clear();
	//activities.resize(size);
	//for (int32_t i = 0; i < size; i++) {
	//	int size2;
	//	loader.retrieve(size2);
	//	for (int32_t j = 0; j < size2; j++) {
	//		int h;
	//		loader.retrieve(h);
	//		activities[i].push_back(WeakReference<Activity, Activity>(h));
	//	}
	//}
}

MovementPaceHandler::MovementPaceHandler(int32_t max) : activities(max), size(max) {
}

MovementPaceHandler::MovementPaceHandler() : MovementPaceHandler(600) {
}

void MovementPaceHandler::finish(GameState& gameState) {
	for (auto& a : activities[i]) {
		a.get()->stopMovement(gameState);
	}
}

void MovementPaceHandler::save(Saver & saver) {
	//saver.store(i);
	//saver.store(size);
	//for (auto& v : activities) {
	//	saver.store<int>(v.size());
	//	for (auto& ref : v) {
	//		saver.store(ref.handle);
	//	}
	//}
}

void MovementPaceHandler::load(Loader & loader) {
	//loader.retrieve(i);
	//loader.retrieve(size);
	//activities.clear();
	//activities.resize(size);
	//for (int32_t i = 0; i < size; i++) {
	//	int size2;
	//	loader.retrieve(size2);
	//	for (int32_t j = 0; j < size2; j++) {
	//		int h;
	//		loader.retrieve(h);
	//		activities[i].push_back(WeakReference<Activity, Activity>(h));
	//	}
	//}
}

void MovementPaceHandler::cycle() {
	activities[i].clear();
	i++;
	i %= size;
}

void MovementPaceHandler::add(WeakReference<Activity, Activity> m, int32_t duration) {
	activities[(i + duration) % size].push_back(m);
}
