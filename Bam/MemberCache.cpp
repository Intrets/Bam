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
	this->validSortedMembers = false;
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

std::vector<Activity*> const& MemberCache::getSortedMembers() {
	if (!this->validSortedMembers) {
		this->sortedMembers = this->getMembers();
		std::sort(this->sortedMembers.begin(), this->sortedMembers.end());
		this->validSortedMembers = true;
	}
	return this->sortedMembers;
}

Activity* MemberCache::getRoot() {
	if (!this->validRoot) {
		this->root = this->self.impl_getRoot();
		this->validRoot = true;
	}
	return this->root;
}

MemberCache::MemberCache(Activity& self_) : self(self_) {
}
