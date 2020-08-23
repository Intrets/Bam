#include "common.h"
#include "ActivityIgnoringGroup.h"
#include "Activity.h"
#include <algorithm>

void ActivityIgnoringGroup::add(Handle h) {
	this->sortedHandles.push_back(h);
}

void ActivityIgnoringGroup::prepare() {
	std::sort(this->sortedHandles.begin(), this->sortedHandles.end());
}

bool ActivityIgnoringGroup::contains(Handle h) const {
	return std::binary_search(this->sortedHandles.begin(), this->sortedHandles.end(), h);
}

ActivityIgnoringGroup::ActivityIgnoringGroup(std::vector<Activity*>& members) {
	for (auto member : members) {
		this->add(member->getHandle());
	}
	this->prepare();
}

ActivityIgnoringGroup::ActivityIgnoringGroup(std::vector<Activity*>& members, std::vector<Activity*>& members2) {
	for (auto member : members) {
		this->add(member->getHandle());
	}
	for (auto member2 : members2) {
		this->add(member2->getHandle());
	}
	this->prepare();
}

ActivityIgnoringGroup::ActivityIgnoringGroup() {
}


ActivityIgnoringGroup::~ActivityIgnoringGroup() {
}
