#pragma once

template<class, class>
class WeakReference;

template<class, class>
class UniqueReference;

template<class>
class ReferenceManager;

class Activity;
class Piston;
class SingleGrouper;
class Anchor;
class GrouperBase;
class RailCrane;
class LuaActivity;
class SingleBlockActivity;
class Grabber;
class Reader;
class Detector;
class Incinerator;
class Forwarder;

typedef int32_t Handle;

class ACTIVITYCOPIER
{
private:
	using HandleMap = std::unordered_map<Handle, Handle>;

	static void copyActivity(ReferenceManager<Activity>& manager, Activity* source, Activity* target, HandleMap& handleMap);
	static void copySingleGrouper(ReferenceManager<Activity>& manager, SingleGrouper* source, SingleGrouper* target, HandleMap& handleMap);
	static void copyGrouperBase(ReferenceManager<Activity>& manager, GrouperBase* source, GrouperBase* target, HandleMap& handleMap);
	static void copySingleBlockActivity(ReferenceManager<Activity>& manager, SingleBlockActivity* source, SingleBlockActivity* target, HandleMap& handleMap);
	static Activity* copyPiston(ReferenceManager<Activity>& manager, Piston* source, HandleMap& handleMap);
	static Activity* copyAnchor(ReferenceManager<Activity>& manager, Anchor* source, HandleMap& handleMap);
	static Activity* copyRailCrane(ReferenceManager<Activity>& manager, RailCrane* source, HandleMap& handleMap);
	static Activity* copyLuaActivity(ReferenceManager<Activity>& manager, LuaActivity* source, HandleMap& handleMap);
	static Activity* copyGrabber(ReferenceManager<Activity>& manager, Grabber* source, HandleMap& handleMap);
	static Activity* copyReader(ReferenceManager<Activity>& manager, Reader* source, HandleMap& handleMap);
	static Activity* copyDetector(ReferenceManager<Activity>& manager, Detector* source, HandleMap& handleMap);
	static Activity* copyIncinerator(ReferenceManager<Activity>& manager, Incinerator* source, HandleMap& handleMap);
	static Activity* copyForwarder(ReferenceManager<Activity>& manager, Forwarder* source, HandleMap& handleMap);

public:
	static UniqueReference<Activity, Activity> copy(ReferenceManager<Activity>& manager, WeakReference<Activity, Activity> ref);
};

