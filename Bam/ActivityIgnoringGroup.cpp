#include "common.h"
#include "ActivityIgnoringGroup.h"
#include "Activity.h"
#include <algorithm>

void ActivityIgnoringGroup::add(Handle h) {
	sortedHandles.push_back(h);
}

void ActivityIgnoringGroup::prepare() {
	std::sort(sortedHandles.begin(), sortedHandles.end());
}

bool ActivityIgnoringGroup::contains(Handle h) {
	return std::binary_search(sortedHandles.begin(), sortedHandles.end(), h);
}

ActivityIgnoringGroup::ActivityIgnoringGroup(std::vector<Activity*>& members) {
	for (auto member : members) {
		add(member->getHandle());
	}
	prepare();
}

ActivityIgnoringGroup::ActivityIgnoringGroup(std::vector<Activity*>& members, std::vector<Activity*>& members2) {
	for (auto member : members) {
		add(member->getHandle());
	}
	for (auto member2 : members2) {
		add(member2->getHandle());
	}
	prepare();
}

ActivityIgnoringGroup::ActivityIgnoringGroup() {
}


ActivityIgnoringGroup::~ActivityIgnoringGroup() {
}
