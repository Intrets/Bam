#include "common.h"
#include "ActivityIgnoringGroup.h"
#include "Activity.h"
#include <algorithm>

bool ActivityIgnoringGroup::contains(Activity* h) const {
	return std::binary_search(this->sortedHandles.begin(), this->sortedHandles.end(), h);
}

ActivityIgnoringGroup::ActivityIgnoringGroup(std::vector<Activity*> const& handles) : sortedHandles(handles) {
}

ActivityIgnoringGroup::~ActivityIgnoringGroup() {
}
