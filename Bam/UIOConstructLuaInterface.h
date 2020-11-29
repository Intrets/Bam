#pragma once

class UIOList;

template<class, class>
class WeakReference;

class LuaActivity;
class Activity;

namespace UIO2
{
	UIOList* constructLuaInterface(WeakReference<Activity, LuaActivity> ref);
}

