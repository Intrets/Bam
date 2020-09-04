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
	int32_t blockID = 0;

	bool occluded = false;

	WeakReference<Activity, Activity> m;

public:
	bool isOccupied(ActivityIgnoringGroup const& ignore);
	bool isOccupied();

	bool isOccluded();

	bool isBlock();

	bool isActivity();

	void setID(int32_t id) {
		this->blockID = id;
	};
	int32_t getID() {
		return this->blockID;
	};
	void setM(WeakReference<Activity, Activity> m_) {
		this->m = m_;
	};
	void setM(Handle m_) {
		this->m = m_;
	};

	Block(int32_t id);

	Block() = default;
	~Block() = default;

	bool load(Loader& loader);
	bool save(Saver& saver);
};

