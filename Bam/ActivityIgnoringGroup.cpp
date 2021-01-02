#include "common.h"
#include "ActivityIgnoringGroup.h"
#include "Activity.h"
#include <algorithm>

bool ActivityIgnoringGroup::contains(Activity* h) const {
	return std::binary_search(this->sortedMembers.begin(), this->sortedMembers.end(), h);
}

ActivityIgnoringGroup::ActivityIgnoringGroup(std::vector<Activity*> const& handles) : sortedMembers(handles) {
}

ActivityIgnoringGroup::~ActivityIgnoringGroup() {
}
