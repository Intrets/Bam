#pragma once

#include "Block.h"
#include "ReferenceManager.h"
#include "Activity.h"

class GameState;

class InventoryItem
{
public:
	Handle selfHandle;

	// get icon render info
	virtual std::string getName() = 0;

	virtual	bool place(GameState& gameState, glm::ivec2 pos) = 0;
	virtual bool canPlace(GameState& gameState, glm::ivec2 pos) = 0;

	InventoryItem() = default;
	virtual ~InventoryItem() = default;

	virtual void addWorldRenderInfo(GameState& gameState, RenderInfo& renderInfo, glm::ivec2 pos) = 0;
};


class InventoryBlock : public InventoryItem
{
private:
	ShapedBlock block;

public:
	virtual std::string getName() override;

	virtual bool place(GameState& gameState, glm::ivec2 pos) override;
	virtual bool canPlace(GameState& gameState, glm::ivec2 pos) override;

	InventoryBlock() = default;
	InventoryBlock(Handle self, ShapedBlock b);
	virtual ~InventoryBlock() = default;

	virtual void addWorldRenderInfo(GameState& gameState, RenderInfo& renderInfo, glm::ivec2 pos) override;
};

class InventoryActivity : public InventoryItem
{
private:
	UniqueReference<Activity, Activity> activity;

public:
	virtual std::string getName() override;

	virtual bool place(GameState& gameState, glm::ivec2 pos) override;
	virtual bool canPlace(GameState& gameState, glm::ivec2 pos) override;

	virtual void addWorldRenderInfo(GameState& gameState, RenderInfo& renderInfo, glm::ivec2 pos) override;
};
