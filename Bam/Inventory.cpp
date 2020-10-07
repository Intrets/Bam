#include "common.h"

#include "Inventory.h"
#include "GameState.h"

std::vector<UniqueReference<InventoryItem, InventoryItem>> const& Inventory::getItems() {
	return this->items;
}

std::optional<UniqueReference<InventoryItem, InventoryItem>> const& Inventory::getCursor() {
	return this->cursor;
}

void Inventory::click(int32_t index) {
	if (indexInVector(index, this->items)) {
		if (this->cursor.has_value()) {
			auto tmp = std::move(this->cursor.value());
			this->cursor = std::move(this->items[index]);
			this->items[index] = std::move(tmp);
		}
		else {
			this->cursor = std::move(this->items[index]);
			this->items.erase(this->items.begin() + index);
		}
	}
	else {
		if (this->cursor.has_value()) {
			this->items.push_back(std::move(this->cursor.value()));
			this->cursor = std::nullopt;
		}
	}
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
	{
		auto block = refMan->makeUniqueRef<InventoryBlock>(ShapedBlock("diorite", SHAPE::TYPE::FORWARDER, ACTIVITY::DIR::RIGHT));
		this->cursor = std::move(block);
	}
}
