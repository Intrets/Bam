#pragma once

#include "ReferenceManager.h"
#include "Activity.h"

class Grouper : public Activity 
{
public:
	Grouper() = default;
	Grouper(Handle self, glm::ivec2 p);
	virtual bool addChild(WeakReference<Activity, Activity> ref) = 0;
};

