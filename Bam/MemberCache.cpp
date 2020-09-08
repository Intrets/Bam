#include "common.h"

#include "MemberCache.h"
#include "Activity.h"

void MemberCache::invalidateAll() {
	this->invalidateMembers();
	this->invalidateRoot();
}

void MemberCache::invalidateMembers() {
	this->membersTick++;

	this->validMembers = false;
	this->validSortedHandles = false;
}

void MemberCache::invalidateRoot() {
	this->rootTick++;

	this->validRoot = false;
}

bool MemberCache::isValidAll() const {
	return this->isValidMembers() && this->isValidRoot();
}

bool MemberCache::isValidMembers() const {
	return this->validMembers;
}

bool MemberCache::isValidRoot() const {
	return this->validRoot;
}

int32_t MemberCache::getMembersTick() const {
	return this->membersTick;
}

int32_t MemberCache::getRootTick() const {
	return this->rootTick;
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
