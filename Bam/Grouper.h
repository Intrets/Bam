#pragma once

#include "ReferenceManager.h"
#include "Activity.h"

class Grouper : public Activity
{
public:
	WeakReference<Activity, Anchor> child;

	Grouper() = default;
	Grouper(Handle self, glm::ivec2 p);
	virtual ~Grouper() = default;

	virtual bool addChild(WeakReference<Activity, Activity> ref);
	virtual void getTreeMembers(std::vector<Activity*>& members);

	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;
};

