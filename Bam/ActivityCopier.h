#pragma once

template<class, class>
class WeakReference;

template<class, class>
class UniqueReference;

class Activity;
class Piston;
class SingleGrouper;
class Anchor;
class GrouperBase;
class RailCrane;
class LuaActivity;
class SingleBlockActivity;
class Grabber;

typedef int32_t Handle;

namespace ACTIVITYCOPIER
{
	using HandleMap = std::unordered_map<Handle, Handle>;

	void copyActivity(Activity* source, Activity* target, HandleMap& handleMap);
	void copySingleGrouper(SingleGrouper* source, SingleGrouper* target, HandleMap& handleMap);
	void copyGrouperBase(GrouperBase* source, GrouperBase* target, HandleMap& handleMap);
	void copySingleBlockActivity(SingleBlockActivity* source, SingleBlockActivity* target, HandleMap& handleMap);
	Activity* copyPiston(Piston* source, HandleMap& handleMap);
	Activity* copyAnchor(Anchor* source, HandleMap& handleMap);
	Activity* copyRailCrane(RailCrane* source, HandleMap& handleMap);
	Activity* copyLuaActivity(LuaActivity* source, HandleMap& handleMap);
	Activity* copyGrabber(Grabber* source, HandleMap& handleMap);

	UniqueReference<Activity, Activity> copy(WeakReference<Activity, Activity> ref);

}

