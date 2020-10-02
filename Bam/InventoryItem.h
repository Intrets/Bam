#pragma once

#include "Block.h"
#include "ReferenceManager.h"
#include "Activity.h"

class GameState;

class InventoryItem
{
public:
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
	virtual bool place(GameState& gameState, glm::ivec2 pos) override;
	virtual bool canPlace(GameState& gameState, glm::ivec2 pos) override;

	InventoryBlock() = default;
	virtual ~InventoryBlock() = default;

	virtual void addWorldRenderInfo(GameState& gameState, RenderInfo& renderInfo, glm::ivec2 pos) override;
};

class InventoryActivity : public InventoryItem
{
private:
	UniqueReference<Activity, Activity> activity;

public:
	virtual bool place(GameState& gameState, glm::ivec2 pos) override;
	virtual bool canPlace(GameState& gameState, glm::ivec2 pos) override;

	virtual void addWorldRenderInfo(GameState& gameState, RenderInfo& renderInfo, glm::ivec2 pos) override;
};
