#include "common.h"

#include "MemberCache.h"
#include "Activity.h"

void MemberCache::invalidate() {
	this->validMembers = false;
	this->validRoot = false;
	this->validSortedHandles = false;
}

std::vector<Activity*> const& MemberCache::getMembers() {
	if (!this->validMembers) {
		this->members.clear();
		this->self.getTreeMembers(this->members);
	}
	return this->members;
}

std::vector<Handle> const& MemberCache::getSortedHandles() {
	if (!this->validSortedHandles) {
		this->sortedHandles.clear();
		for (auto member : this->getMembers()) {
			this->sortedHandles.push_back(member->getHandle());
		}
		std::sort(this->sortedHandles.begin(), this->sortedHandles.end());
	}
	return this->sortedHandles;
}

Activity* MemberCache::getRoot() {
	if (!this->validRoot) {
		this->root = this->self.getRoot().get();
	}
	return this->root;
}

MemberCache::MemberCache(Activity& self_) : self(self_) {
}
