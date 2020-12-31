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

UniqueReference<Activity, Activity> const& SingleGrouper::getChild() {
	return this->child;
}

UniqueReference<Activity, Activity> SingleGrouper::popChild() {
	return std::move(this->child);
}

bool SingleGrouper::addChild(UniqueReference<Activity, Activity> ref) {
	if (this->child.isNotNull()) {
		return false;
	}
	else {
		ref.get()->parentRef = WeakReference<Activity, GrouperBase>(*ref.getManager(), this);
		this->child = std::move(ref);
		return true;
	}
}

std::optional<UniqueReference<Activity, Activity>> SingleGrouper::removeChild(WeakReference<Activity, Activity> ref) {
	if (this->child == ref) {
		return std::move(this->child);
	}
	else {
		return std::nullopt;
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

void SingleGrouper::impl_getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) {
	members.push_back({ depth, this });
	if (this->child.isNotNull()) {
		this->child.get()->impl_getTreeMembersDepths(members, depth + 1);
	}
}

void SingleGrouper::save(Saver& saver) {
	this->GrouperBase::save(saver);
	saver.store(this->child);
}

bool SingleGrouper::load(Loader& loader) {
	this->GrouperBase::load(loader);
	loader.retrieve(this->child);
	return true;
}
