#include "common.h"
#include "Grouper.h"

Grouper::Grouper(Handle self, glm::ivec2 p) : Activity(self, p){
}

//bool Grouper::addChild(WeakReference<Activity, Activity> ref) {
//	if (child.isNotNull()) {
//		return false;
//	}
//	else {
//		child = ref;
//	}
//	return true;
//}
