#pragma once

#include "Block.h"
#include "Enums.h"
#include <optional>
#include "Activity.h"

class Saver;
class Loader;

template<class, class>
class WeakReference;

class Activity;
class ActivityIgnoringGroup;

typedef int32_t Handle;

class WorldBlock
{
private:
	ShapedBlock shapedBlock;

	//WeakReference<Activity, Activity> m;
	Activity* m = nullptr;

public:
	// returns if block is occupied by an activity or a solid block
	bool isOccupied(ActivityIgnoringGroup const& ignore);
	// returns if block is occupied by an activity or a solid block
	bool isOccupied();

	bool isSolid();
	bool isActivity();
	bool isNonAirBlock();

	std::optional<WeakReference<Activity, Activity>> getActivityMaybe() const;
	WeakReference<Activity, Activity> getActivity() const;
	ACTIVITY::DIR getRotation() const;
	ShapedBlock const& getShapedBlock() const;

	void setBlock(ShapedBlock block);
	void setTrace(Activity* h);
	void removeTrace();
	void removeTrace(Activity* h);

	WorldBlock(std::string block, std::string shape);
	WorldBlock(int32_t blockID);
	WorldBlock(int32_t blockID, int32_t shapeID);
	WorldBlock(int32_t blockID, int32_t shapeID, ACTIVITY::DIR rotation);

	WorldBlock() = default;
	~WorldBlock() = default;

	NOCOPYMOVE(WorldBlock);

	bool load(Loader& loader);
	bool save(Saver& saver);
};
