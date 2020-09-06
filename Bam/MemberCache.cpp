#include "common.h"

#include "MemberCache.h"
#include "Activity.h"

void MemberCache::invalidateAll() {
	this->validMembers = false;
	this->validRoot = false;
	this->validSortedHandles = false;
}

void MemberCache::invalidateMembers() {
	this->validMembers = false;
	this->validSortedHandles = false;
}

void MemberCache::invalidateRoot() {
	this->validRoot = false;
}

std::vector<Activity*> const& MemberCache::getMembers() {
	if (!this->validMembers) {
		this->members.clear();
		this->self.impl_getTreeMembers(this->members);
		this->validMembers = true;
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
		this->validSortedHandles = true;
	}
	return this->sortedHandles;
}

Activity* MemberCache::getRoot() {
	if (!this->validRoot) {
		this->root = WeakReference<Activity, Activity>(this->self.impl_getRootHandle()).get();
		this->validRoot = true;
	}
	return this->root;
}

MemberCache::MemberCache(Activity& self_) : self(self_) {
}
