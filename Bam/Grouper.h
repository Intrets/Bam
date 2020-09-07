#pragma once

#include "ReferenceManager.h"
#include "Activity.h"

class GrouperBase : public Activity
{
public:
	GrouperBase() = default;
	GrouperBase(Handle self, glm::ivec2 p) : Activity(self, p) {
	};
	virtual ~GrouperBase() = default;

	virtual bool addChild(Handle h);
	virtual bool addChild(WeakReference<Activity, Activity> ref) = 0;
	virtual bool removeChild(Handle h);
	virtual bool removeChild(WeakReference<Activity, Activity> ref) = 0;
	virtual bool hasChild() const = 0;

	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;
};

class SingleGrouper : public GrouperBase
{
protected:
	WeakReference<Activity, Activity> child;

	friend void ACTIVITYCOPIER::copySingleGrouper(SingleGrouper*, SingleGrouper*, HandleMap&);

public:
	WeakReference<Activity, Activity> const& getChild();

	virtual bool addChild(WeakReference<Activity, Activity> ref) override;
	virtual bool removeChild(WeakReference<Activity, Activity> ref) override;
	virtual bool hasChild() const override;

	SingleGrouper() = default;
	SingleGrouper(Handle self, glm::ivec2 p) : GrouperBase(self, p) {
	};
	virtual ~SingleGrouper() = default;

	// Tree Information

	// non-cached base implementation
	virtual void impl_getTreeMembers(std::vector<Activity*>& members) override;
	// non-cached base implementation
	virtual void impl_getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) override;

	// Serial
	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;
};

