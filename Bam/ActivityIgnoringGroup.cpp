#include "common.h"
#include "ActivityIgnoringGroup.h"
#include "Activity.h"
#include <algorithm>

bool ActivityIgnoringGroup::contains(Handle h) const {
	return std::binary_search(this->sortedHandles.begin(), this->sortedHandles.end(), h);
}

ActivityIgnoringGroup::ActivityIgnoringGroup(std::vector<Handle> const& handles) : sortedHandles(handles) {
}

ActivityIgnoringGroup::~ActivityIgnoringGroup() {
}
