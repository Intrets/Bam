#pragma once

// #include "Activity.h"
// #include "Loader.h"
// #include "Saver.h"

class ActivityIgnoringGroup;

class Block
{
private:
	friend class StaticWorldChunk;
	// 0  = empty
	// 1  = activity, reference m
	// 1+ = block, value being block ID 

	int blockID = 0;
	bool occluded = false;

	//WeakReference<Activity, Activity> m;

	//bool load(Loader& loader);
	//bool save(Saver& saver);

public:
	bool isOccupied(ActivityIgnoringGroup& ignore);
	bool isOccupied();
	bool isOccluded();
	bool isBlock();
	bool isActivity();

	Block(int id);

	Block() = default;
	~Block() = default;
};

