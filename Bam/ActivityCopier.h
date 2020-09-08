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

class ACTIVITYCOPIER
{
private:
	using HandleMap = std::unordered_map<Handle, Handle>;

	static void copyActivity(Activity* source, Activity* target, HandleMap& handleMap);
	static void copySingleGrouper(SingleGrouper* source, SingleGrouper* target, HandleMap& handleMap);
	static void copyGrouperBase(GrouperBase* source, GrouperBase* target, HandleMap& handleMap);
	static void copySingleBlockActivity(SingleBlockActivity* source, SingleBlockActivity* target, HandleMap& handleMap);
	static Activity* copyPiston(Piston* source, HandleMap& handleMap);
	static Activity* copyAnchor(Anchor* source, HandleMap& handleMap);
	static Activity* copyRailCrane(RailCrane* source, HandleMap& handleMap);
	static Activity* copyLuaActivity(LuaActivity* source, HandleMap& handleMap);
	static Activity* copyGrabber(Grabber* source, HandleMap& handleMap);

public:
	static UniqueReference<Activity, Activity> copy(WeakReference<Activity, Activity> ref);
};

