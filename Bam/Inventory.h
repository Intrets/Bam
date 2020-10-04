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
	std::vector<UniqueReference<InventoryItem, InventoryItem>> const& getItems();

	Inventory();
	~Inventory() = default;
};

