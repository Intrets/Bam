#include "common.h"

#include "Grouper.h"

#include "Anchor.h"
#include "Saver.h"
#include "Loader.h"

Grouper::Grouper(Handle self, glm::ivec2 p) : Activity(self, p) {
}

bool Grouper::addChild(WeakReference<Activity, Activity> ref) {
	if (this->child.isNotNull()) {
		return false;
	}
	else {
		this->child = ref;
	}
	return true;
}

void Grouper::getTreeMembers(std::vector<Activity*>& members) {
	members.push_back(this);
	if (this->child.isNotNull()) {
		this->child.get()->getTreeMembers(members);
	}
}

void Grouper::getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) {
	members.push_back({ depth, this });
	if (this->child.isNotNull()) {
		this->child.get()->getTreeMembersDepths(members, depth + 1);
	}
}

void Grouper::save(Saver& saver) {
	Activity::save(saver);
	saver.store(this->child.handle);
}

bool Grouper::load(Loader& loader) {
	Activity::load(loader);
	loader.retrieve(this->child.handle);
	return true;
}
