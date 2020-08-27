#pragma once

template<class T, class S>
class WeakReference;

template<class T, class S>
class UniqueReference;

class Activity;

bool sameGroup(WeakReference<Activity, Activity> a1, WeakReference<Activity, Activity> a2);
