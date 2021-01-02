#pragma once

#include "InventoryItem.h"
#include "Enums.h"
#include "ReferenceManager.h"

#include <optional>

class GameState;
struct RenderInfo;
class Saver;
class Loader;

class Inventory
{
private:
	std::vector<UniqueReference<InventoryItem, InventoryItem>> items;

	std::optional<UniqueReference<InventoryItem, InventoryItem>> cursor;

	std::vector<std::optional<UniqueReference<InventoryItem, InventoryItem>>> hotbar;

	std::vector<InventoryActivity const*> listActivities();
	std::vector<InventoryBlock const*> listBlocks();

public:
	std::vector<std::optional<UniqueReference<InventoryItem, InventoryItem>>> const& getHotbar();
	std::vector<UniqueReference<InventoryItem, InventoryItem>> const& getItems();
	std::optional<UniqueReference<InventoryItem, InventoryItem>> const& getCursor();

	bool canExtract(std::vector<std::pair<ShapedBlock, int32_t>> collection);
	bool extract(std::vector<std::pair<ShapedBlock, int32_t>> const& collection);
	bool extractForce(std::vector<std::pair<ShapedBlock, int32_t>> collection);

	bool addItem(UniqueReference<InventoryItem, InventoryItem>& item);
	bool addItemCursor(UniqueReference<InventoryItem, InventoryItem>& item);

	bool hasSpace() const;

	void clickHotbar(int32_t index);

	std::pair<bool, std::optional<Activity*>> clickWorld(GameState& gameState, glm::vec2 pos);
	void clickInventory(int32_t index);
	void pickupWorld(GameState& gameState, glm::vec2 pos);
	void rotateCursorItem(ACTIVITY::ROT rot);
	void deselectCursor();

	Inventory();
	~Inventory() = default;

	void save(Saver& saver);
	void load(Loader& loader);
};

