#pragma once

#include "InventoryItem.h"
#include "Enums.h"
#include "ReferenceManager.h"

#include <optional>

class GameState;
struct RenderInfo;

class Inventory
{
private:
	std::vector<UniqueReference<InventoryItem, InventoryItem>> items;

	std::optional<UniqueReference<InventoryItem, InventoryItem>> cursor;

	std::vector<UniqueReference<InventoryItem, InventoryItem>> hotbar;

public:
	std::vector<UniqueReference<InventoryItem, InventoryItem>> const& getHotbar();
	std::vector<UniqueReference<InventoryItem, InventoryItem>> const& getItems();
	std::optional<UniqueReference<InventoryItem, InventoryItem>> const& getCursor();

	void clickHotbar(int32_t index);
	void clickWorld(GameState& gameState, glm::vec2 pos);
	void clickInventory(int32_t index);

	Inventory();
	~Inventory() = default;
};

