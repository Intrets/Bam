#include "common.h"
#include "Grouper.h"

#include "Anchor.h"

Grouper::Grouper(Handle self, glm::ivec2 p) : Activity(self, p) {
}

bool Grouper::addChild(WeakReference<Activity, Activity> ref) {
	if (child.isNotNull()) {
		return false;
	}
	else {
		child = ref;
	}
	return true;
}

void Grouper::getTreeMembers(std::vector<Activity*>& members) {
	members.push_back(this);
	if (child.isNotNull()) {
		child.get()->getTreeMembers(members);
	}
}

void Grouper::save(Saver& saver) {
	Activity::save(saver);
	saver.store<int>(child.handle);
}

bool Grouper::load(Loader& loader) {
	Activity::load(loader);
	loader.retrieve(child.handle);
	return true;
}
