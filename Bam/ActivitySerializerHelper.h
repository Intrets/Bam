#pragma once

class Saver;
class Loader;
class Activity;

template<class>
class ReferenceManager;

namespace ACTIVITYSERIALIZER
{
	void load(Loader& loader, ReferenceManager<Activity>& manager);
	void save(Saver& saver, ReferenceManager<Activity>& manager);
}
