#pragma once

class Activity;
class Loader;

template<class>
class ReferenceManager;

namespace ACTIVITYLOADER
{
	void load(Loader& loader, ReferenceManager<Activity>& manager);
}
