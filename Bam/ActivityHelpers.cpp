#include "common.h"

#include "ActivityHelpers.h"
#include "ReferenceManager.h"
#include "Activity.h"
#include "Anchor.h"

bool sameGroup(WeakReference<Activity, Activity> a1, WeakReference<Activity, Activity> a2) {
	return a1.get()->getRootHandle() == a2.get()->getRootHandle();
}

