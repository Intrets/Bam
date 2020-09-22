#pragma once

#include "Activity.h"

class ActivityIgnoringGroup;
class Loader;
class Saver;

#define MAXBLOCKS 1000

namespace ELEMENT
{
	enum TYPE
	{
		AIR,
		SILICONDIOXIDE,
		IRON,
		IRONOXIDE,
		CARBON,
		HYDROCARBON,
	};

	static std::unordered_map<std::string, ELEMENT::TYPE> typeMap = {
		{"air", AIR},
		{"silicondioxide", SILICONDIOXIDE},
		{"iron", IRON},
		{"ironoxide", IRONOXIDE},
		{"carbon", CARBON},
		{"hydrocarbon", HYDROCARBON},
	};

	ELEMENT::TYPE getType(std::string s);
	std::string getName(ELEMENT::TYPE type);
}

struct Element
{
	ELEMENT::TYPE type;
	int32_t quantity;
};

struct Material
{
	std::vector<Element> elements;
};

struct BlockData
{
	bool solid = false;
	int32_t texture = 0;
	std::string name = "air";

	Material material;
};

class Block
{
private:
	static std::array<BlockData, MAXBLOCKS> data;
	static std::unordered_map<std::string, int32_t> nameMap;
	static int32_t blockCount;
	BlockData* operator->();

	// 0 = air
	int32_t blockID = 0;
	ACTIVITY::DIR rotation = ACTIVITY::DIR::RIGHT;
	WeakReference<Activity, Activity> m;

	// taken from BlockData
	bool solid = false;

public:
	static void loadBlocks();
	static int32_t getBlockCount();
	static int32_t getBlockID(std::string name);

	// returns if block is occupied by an activity or a solid block
	bool isOccupied(ActivityIgnoringGroup const& ignore);
	// returns if block is occupied by an activity or a solid block
	bool isOccupied();

	bool isSolid();
	bool isActivity();
	bool isNonAirBlock();

	void setBlockID(int32_t id);
	void setBlockID(int32_t id, ACTIVITY::DIR rotation_);

	int32_t getBlockID();
	int32_t getTexture();

	std::string const& getBlockName();

	std::optional<WeakReference<Activity, Activity>> getActivityMaybe() const;
	WeakReference<Activity, Activity> getActivity() const;
	ACTIVITY::DIR getRotation() const;

	void setTrace(Handle h);
	void removeTrace();
	void removeTrace(Handle h);

	Block(int32_t id);
	Block(int32_t id, ACTIVITY::DIR rotation_);

	Block() = default;
	~Block() = default;

	// move
	Block(Block&& other);
	Block& operator=(Block&& other);

	// copy
	Block(const Block&) = default;
	Block& operator=(const Block&) = default;

	bool load(Loader& loader);
	bool save(Saver& saver);
};

