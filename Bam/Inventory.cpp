#include "common.h"

#include "Inventory.h"
#include "GameState.h"
#include "Piston.h"
#include "WorldBlock.h"
#include "Block.h"
#include "Saver.h"
#include "Loader.h"

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

std::pair<bool, std::optional<Activity*>> Inventory::clickWorld(GameState& gameState, glm::vec2 pos) {
	std::pair<bool, std::optional<Activity*>> res = { false, std::nullopt };
	if (this->cursor.has_value()) {
		if (this->cursor.value().get()->place(gameState, pos)) {
			if (this->cursor.value().get()->getType() == INVENTORYITEM::TYPE::ACTIVITY) {
				res = { true, static_cast<InventoryActivity*>(this->cursor.value().get())->getActivityPtr() };
			}
			else {
				res = { true, std::nullopt };
			}
			this->cursor.value().clear();
			this->cursor = std::nullopt;
		}
	}
	return res;
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

void Inventory::save(Saver& saver) {
	saver.store(this->items.size());
	for (auto& item : items) {
		saver.store(item.handle);
	}

	bool hasCursor = this->cursor.has_value();
	saver.store(hasCursor);
	if (hasCursor) {
		saver.store(this->cursor.value().handle);
	}

	saver.store(this->hotbar.size());
	for (auto& item : hotbar) {
		saver.store(item.has_value());
		if (item.has_value()) {
			saver.store(item.value().handle);
		}
	}
}

void Inventory::load(Loader& loader) {
	size_t size;
	loader.retrieve(size);
	this->items.clear();
	for (size_t i = 0; i < size; i++) {
		Handle handle;
		loader.retrieve(handle);
		this->items.emplace_back(handle);
	}

	bool hasCursor;
	loader.retrieve(hasCursor);
	if (hasCursor) {
		Handle handle;
		loader.retrieve(handle);
		this->cursor.emplace(handle);
	}
	else {
		this->cursor = std::nullopt;
	}

	this->hotbar.clear();
	loader.retrieve(size);
	for (size_t i = 0; i < size; i++) {
		bool hasValue;
		loader.retrieve(hasValue);
		if (hasValue) {
			Handle handle;
			loader.retrieve(handle);
			this->hotbar.emplace_back(handle);
		}
		else {
			this->hotbar.push_back(std::nullopt);
		}
	}
}
