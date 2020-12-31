#pragma once

class UIOList;
class UIOBase;

template<class, class>
class WeakReference;

class LuaActivity;
class Activity;

namespace UIO2
{
	WeakReference<UIOBase, UIOList> constructLuaInterface(WeakReference<Activity, LuaActivity> ref);
}

