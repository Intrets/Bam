#include "common.h"

#include "Grouper.h"

#include "Anchor.h"
#include "Saver.h"
#include "Loader.h"

void GrouperBase::save(Saver& saver) {
	this->Activity::save(saver);
}

bool GrouperBase::load(Loader& loader) {
	return this->Activity::load(loader);
}

bool GrouperBase::addChild(Handle h) {
	return this->addChild(WeakReference<Activity, Activity>(h));
}

bool GrouperBase::removeChild(Handle h) {
	return this->removeChild(WeakReference<Activity, Activity>(h));
}

WeakReference<Activity, Activity> const& SingleGrouper::getChild() {
	return this->child;
}

bool SingleGrouper::addChild(WeakReference<Activity, Activity> ref) {
	if (this->child.isNotNull()) {
		return false;
	}
	else {
		this->child = ref;
		ref.get()->parentRef.handle = this->selfHandle;
		return true;
	}
}

bool SingleGrouper::removeChild(WeakReference<Activity, Activity> ref) {
	if (this->child.handle == ref.handle) {
		this->child.clear();
		return true;
	}
	else {
		return false;
	}
}

bool SingleGrouper::hasChild() const {
	return this->child.isNotNull();
}

void SingleGrouper::impl_getTreeMembers(std::vector<Activity*>& members) {
	members.push_back(this);
	if (this->child.isNotNull()) {
		this->child.get()->impl_getTreeMembers(members);
	}
}

void SingleGrouper::getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) {
	members.push_back({ depth, this });
	if (this->child.isNotNull()) {
		this->child.get()->getTreeMembersDepths(members, depth + 1);
	}
}

void SingleGrouper::save(Saver& saver) {
	this->GrouperBase::save(saver);
	saver.store(this->child.handle);
}

bool SingleGrouper::load(Loader& loader) {
	this->GrouperBase::load(loader);
	loader.retrieve(this->child.handle);
	return true;
}
