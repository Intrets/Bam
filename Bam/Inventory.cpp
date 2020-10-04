#include "common.h"

#include "Inventory.h"
#include "GameState.h"

std::vector<UniqueReference<InventoryItem, InventoryItem>> const& Inventory::getItems() {
	return this->items;
}

Inventory::Inventory() {
	auto refMan = Locator<ReferenceManager<InventoryItem>>::get();
	{
		auto block = refMan->makeUniqueRef<InventoryBlock>(ShapedBlock("diorite", SHAPE::TYPE::DETECTOR, ACTIVITY::DIR::RIGHT));
		this->items.push_back(std::move(block));
	}
	{
		auto block = refMan->makeUniqueRef<InventoryBlock>(ShapedBlock("diorite", SHAPE::TYPE::FORWARDER, ACTIVITY::DIR::RIGHT));
		this->items.push_back(std::move(block));
	}
}
