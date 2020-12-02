#pragma once

#include "Block.h"
#include "ReferenceManager.h"
#include "Activity.h"
#include "Enums.h"

class GameState;
class Saver;
class Loader;

class InventoryItem
{
public:
	Handle selfHandle;

	// get icon render info
	virtual std::string getName() = 0;
	virtual INVENTORYITEM::TYPE getType() = 0;

	// returns true if the item is used up and should be cleaned up
	virtual	bool place(GameState& gameState, glm::ivec2 pos) = 0;
	virtual bool canPlace(GameState& gameState, glm::ivec2 pos) = 0;
	virtual void rotate(ACTIVITY::ROT rot) = 0;
	virtual void setOrientation(ACTIVITY::DIR dir) = 0;

	InventoryItem() = default;
	virtual ~InventoryItem() = default;

	virtual void addWorldRenderInfo(GameState& gameState, RenderInfo& renderInfo, glm::ivec2 pos) = 0;

	virtual void save(Saver& saver);
	virtual void load(Loader& loader);
};

class InventoryBlock : public InventoryItem
{
private:
	int32_t count;
	ShapedBlock block;

public:
	ShapedBlock const& getBlock() const;
	bool incrementCount(int32_t c);
	void decrementCount(int32_t c);

	int32_t getCount() const;

	virtual std::string getName() override;
	virtual INVENTORYITEM::TYPE getType() override;

	virtual bool place(GameState& gameState, glm::ivec2 pos) override;
	virtual bool canPlace(GameState& gameState, glm::ivec2 pos) override;
	virtual void rotate(ACTIVITY::ROT rot) override;
	virtual void setOrientation(ACTIVITY::DIR dir) override;

	InventoryBlock() = default;
	InventoryBlock(Handle self, ShapedBlock b, int32_t c);
	virtual ~InventoryBlock() = default;

	virtual void addWorldRenderInfo(GameState& gameState, RenderInfo& renderInfo, glm::ivec2 pos) override;
	virtual void save(Saver& saver) override;
	virtual void load(Loader& loader) override;
};

class InventoryActivity : public InventoryItem
{
private:
	UniqueReference<Activity, Activity> activity;

public:
	Activity* getActivityPtr();

	InventoryActivity() = default;
	InventoryActivity(Handle self, UniqueReference<Activity, Activity> a);
	~InventoryActivity() = default;

	virtual std::string getName() override;
	virtual INVENTORYITEM::TYPE getType() override;

	virtual bool place(GameState& gameState, glm::ivec2 pos) override;
	virtual bool canPlace(GameState& gameState, glm::ivec2 pos) override;
	virtual void rotate(ACTIVITY::ROT rot) override;
	virtual void setOrientation(ACTIVITY::DIR dir) override;

	virtual void addWorldRenderInfo(GameState& gameState, RenderInfo& renderInfo, glm::ivec2 pos) override;
	virtual void save(Saver& saver) override;
	virtual void load(Loader& loader) override;
};
