#include "common.h"

#include "Inventory.h"
#include "GameState.h"
#include "Piston.h"
#include "WorldBlock.h"
#include "Block.h"

std::vector<std::optional<UniqueReference<InventoryItem, InventoryItem>>> const& Inventory::getHotbar() {
	return this->hotbar;
}

std::vector<UniqueReference<InventoryItem, InventoryItem>> const& Inventory::getItems() {
	return this->items;
}

std::optional<UniqueReference<InventoryItem, InventoryItem>> const& Inventory::getCursor() {
	return this->cursor;
}

void Inventory::clickHotbar(int32_t index) {
	if (indexInVector(index, this->hotbar)) {
		if (this->cursor.has_value()) {
			auto tmp = std::move(this->cursor.value());
			if (this->hotbar[index].has_value()) {
				this->cursor = std::move(this->hotbar[index]);
				this->cursor.value().get()->setOrientation(ACTIVITY::DIR::RIGHT);
			}
			else {
				this->cursor = std::nullopt;
			}
			this->hotbar[index] = std::move(tmp);
		}
		else {
			if (this->hotbar[index].has_value()) {
				this->cursor = std::move(this->hotbar[index]);
				this->cursor.value().get()->setOrientation(ACTIVITY::DIR::RIGHT);
				this->hotbar[index] = std::nullopt;
			}
		}
	}
}

void Inventory::clickWorld(GameState& gameState, glm::vec2 pos) {
	if (this->cursor.has_value()) {
		if (cursor.value().get()->place(gameState, pos)) {
			this->cursor.value().clear();
			this->cursor = std::nullopt;
		}
	}
}

void Inventory::clickInventory(int32_t index) {
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
		this->cursor.value().get()->setOrientation(ACTIVITY::DIR::RIGHT);
	}
	else {
		if (this->cursor.has_value()) {
			this->items.push_back(std::move(this->cursor.value()));
			this->cursor = std::nullopt;
		}
	}
}

void Inventory::pickupWorld(GameState& gameState, glm::vec2 pos) {
	if (this->cursor.has_value()) {
		return;
	}
	auto& target = gameState.staticWorld.getBlockRef(pos);
	if (target.isActivity()) {
		auto root = target.getActivity().get()->getRootPtr();
		if (root->removeTracesUp(gameState)) {
			this->cursor = Locator<ReferenceManager<InventoryItem>>::ref().makeUniqueRef<InventoryActivity>(UniqueReference<Activity, Activity>(root->getHandle()));
		}
	}
	else if (target.isNonAirBlock()) {
		this->cursor = Locator<ReferenceManager<InventoryItem>>::ref().makeUniqueRef<InventoryBlock>(target.getShapedBlock(), 1);
		target.setBlock(ShapedBlock());
	}
}

void Inventory::rotateCursorItem(ACTIVITY::ROT rot) {
	if (this->cursor.has_value()) {
		this->cursor.value().get()->rotate(rot);
	}
}

Inventory::Inventory() {
	auto refMan = Locator<ReferenceManager<InventoryItem>>::get();
	{
		auto block = refMan->makeUniqueRef<InventoryBlock>(ShapedBlock("diorite", SHAPE::TYPE::DETECTOR, ACTIVITY::DIR::RIGHT), 10);
		this->items.push_back(std::move(block));
	}
	{
		auto block = refMan->makeUniqueRef<InventoryBlock>(ShapedBlock("diorite", SHAPE::TYPE::FORWARDER, ACTIVITY::DIR::RIGHT), 10);
		this->items.push_back(std::move(block));
	}
	{
		auto block = refMan->makeUniqueRef<InventoryBlock>(ShapedBlock("diorite", SHAPE::TYPE::FORWARDER, ACTIVITY::DIR::RIGHT), 10);
		this->cursor = std::move(block);
	}
	{
		auto activity = Locator<ReferenceManager<Activity>>::ref().makeUniqueRef<Piston>(glm::ivec2(0, 0), ACTIVITY::DIR::RIGHT);
		auto activityItem = refMan->makeUniqueRef<InventoryActivity>(std::move(activity));
		this->items.push_back(std::move(activityItem));
	}

	this->hotbar.reserve(10);
	for (size_t i = 0; i < 10; i++) {
		this->hotbar.push_back(std::nullopt);
	}
}
