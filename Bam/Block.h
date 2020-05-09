#pragma once

 #include "Activity.h"

class ActivityIgnoringGroup;
class Loader;
class Saver;

class Block
{
private:
	friend class StaticWorldChunk;
	friend class StaticWorld;

	// 0  = air
	// 1+ = solid block
	int blockID = 0;

	bool occluded = false;

	WeakReference<Activity, Activity> m;

	bool load(Loader& loader);
	bool save(Saver& saver);

public:
	bool isOccupied(ActivityIgnoringGroup& ignore);
	bool isOccupied();

	bool isOccluded();

	bool isBlock();

	bool isActivity();

	void setID(int id) { blockID = id; };
	int getID() { return blockID; };
	void setM(WeakReference<Activity, Activity> m_) { m = m_; };

	Block(int id);

	Block() = default;
	~Block() = default;
};

