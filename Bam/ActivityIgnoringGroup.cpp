#include "common.h"
#include "ActivityIgnoringGroup.h"
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

ActivityIgnoringGroup::ActivityIgnoringGroup() {
}


ActivityIgnoringGroup::~ActivityIgnoringGroup() {
}
