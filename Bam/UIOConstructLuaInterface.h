#pragma once

template<class>
class UIOConstructer;

class UIOList;

template<class, class>
class WeakReference;

class LuaActivity;
class Activity;

namespace CONSTRUCTER
{
	UIOConstructer<UIOList> constructLuaInterface(WeakReference<Activity, LuaActivity> ref);
}
