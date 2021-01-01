#include "common.h"

#include "Inventory.h"
#include "GameState.h"
#include "Piston.h"
#include "WorldBlock.h"
#include "Block.h"
#include "Saver.h"
#include "Loader.h"

std::vector<InventoryActivity const*> Inventory::listActivities() {
	std::vector<InventoryActivity const*> res;

	for (auto& item : this->items) {
		if (item.get()->getType() == INVENTORYITEM::TYPE::ACTIVITY) {
			res.push_back(item.getAs<InventoryActivity>());
		}
	}

	for (auto& item : this->hotbar) {
		if (item && item.value().get()->getType() == INVENTORYITEM::TYPE::ACTIVITY) {
			res.push_back(item.value().getAs<InventoryActivity>());
		}
	}

	return res;
}

std::vector<InventoryBlock const*> Inventory::listBlocks() {
	std::vector<InventoryBlock const*> res;

	for (auto& item : this->items) {
		if (item.get()->getType() == INVENTORYITEM::TYPE::BLOCK) {
			res.push_back(item.getAs<InventoryBlock>());
		}
	}

	for (auto& item : this->hotbar) {
		if (item && item.value().get()->getType() == INVENTORYITEM::TYPE::BLOCK) {
			res.push_back(item.value().getAs<InventoryBlock>());
		}
	}

	return res;
}

std::vector<std::optional<UniqueReference<InventoryItem, InventoryItem>>> const& Inventory::getHotbar() {
	return this->hotbar;
}

std::vector<UniqueReference<InventoryItem, InventoryItem>> const& Inventory::getItems() {
	return this->items;
}

std::optional<UniqueReference<InventoryItem, InventoryItem>> const& Inventory::getCursor() {
	return this->cursor;
}

bool Inventory::canExtract(std::vector<std::pair<ShapedBlock, int32_t>> collection) {
	auto inventoryBlocks = this->listBlocks();

	for (auto [block, count] : collection) {
		for (auto inventoryBlock : inventoryBlocks) {
			if (block == inventoryBlock->getBlock()) {
				count -= inventoryBlock->getCount();
				count = glm::max(0, count);
			}

			if (count == 0) {
				break;
			}
		}
		if (count != 0) {
			return false;
		}
	}

	return true;
}

bool Inventory::extract(std::vector<std::pair<ShapedBlock, int32_t>> const& collection) {
	if (!this->canExtract(collection)) {
		return false;
	}
	else {
		this->extractForce(collection);
		return true;
	}
}

bool Inventory::extractForce(std::vector<std::pair<ShapedBlock, int32_t>> collection) {
	for (auto [block, count] : collection) {
		for (auto it = this->items.begin(); it != this->items.end();) {
			auto item = it->get();
			if (item->getType() == INVENTORYITEM::TYPE::BLOCK) {
				auto blockItem = static_cast<InventoryBlock*>(item);
				if (blockItem->getBlock() == block) {
					if (blockItem->getCount() <= count) {
						count -= blockItem->getCount();
						it = this->items.erase(it);
						continue;
					}
					else {
						blockItem->decrementCount(count);
						count = 0;
						break;
					}
				}
			}
			it++;
		}

		if (count == 0) {
			continue;
		}

		for (auto it = this->hotbar.begin(); it != this->hotbar.end(); it++) {
			if (!it->has_value()) {
				continue;
			}

			auto item = it->value().get();

			if (item->getType() == INVENTORYITEM::TYPE::BLOCK) {
				auto blockItem = static_cast<InventoryBlock*>(item);
				if (blockItem->getBlock() == block) {
					if (blockItem->getCount() <= count) {
						count -= blockItem->getCount();
						*it = std::nullopt;
						continue;
					}
					else {
						blockItem->decrementCount(count);
						count = 0;
						break;
					}
				}
			}
		}

		assert(count == 0);
	}

	return false;
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
		if (this->cursor.value().get()->getType() == INVENTORYITEM::TYPE::ACTIVITY) {
			auto inventoryActivity = this->cursor.value().getAs<InventoryActivity>();
			auto activity = inventoryActivity->getActivityPtr();

			if (inventoryActivity->place(gameState, pos)) {
				res = { true, activity };
				this->cursor = std::nullopt;
			}
			else {
				res = { false, std::nullopt };
			}
		}
		else {
			auto block = this->cursor.value().getAs<InventoryBlock>();

			if (block->place(gameState, pos)) {
				res = { true, std::nullopt };
				this->cursor = std::nullopt;
			}
			else {
				res = { false, std::nullopt };
			}
		}
	}
	return res;
}

void Inventory::clickInventory(int32_t index) {
	if (indexInVector(index, this->items)) {
		if (this->cursor.has_value()) {
			auto tmp = std::move(this->cursor.value());
			this->cursor = std::move(this->items[index]);
			this->items.erase(this->items.begin() + index);
			if (!this->addItem(tmp)) {
				this->items.insert(this->items.begin() + index, std::move(this->cursor.value()));
				this->cursor = std::move(tmp);
			}
		}
		else {
			this->cursor = std::move(this->items[index]);
			this->items.erase(this->items.begin() + index);
		}
		this->cursor.value().get()->setOrientation(ACTIVITY::DIR::RIGHT);
	}
	else {
		if (this->cursor.has_value()) {
			if (this->addItem(this->cursor.value())) {
				this->cursor = std::nullopt;
			}
		}
	}
}

void Inventory::pickupWorld(GameState& gameState, glm::vec2 pos) {
	if (this->cursor.has_value()) {
		return;
	}
	auto& target = gameState.staticWorld.getBlockRef(pos);
	if (target.isActivity()) {
		auto root = target.getActivity().get()->getRootRef();
		if (root.get()->removeTracesUp(gameState)) {
			this->cursor = Locator<ReferenceManager<InventoryItem>>::ref().makeUniqueRef<InventoryActivity>(UniqueReference<Activity, Activity>(root));
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

void Inventory::deselectCursor() {
	if (this->cursor.has_value()) {
		if (this->addItem(this->cursor.value())) {
			this->cursor = std::nullopt;
		}
	}
}

Inventory::Inventory() {
	for (size_t i = 0; i < 10; i++) {
		this->hotbar.push_back(std::nullopt);
	}
}

bool Inventory::addItem(UniqueReference<InventoryItem, InventoryItem>& item) {
	switch (item.get()->getType()) {
		case INVENTORYITEM::TYPE::ACTIVITY:
			{
				this->items.push_back(std::move(item));
				return true;
				break;
			}
		case INVENTORYITEM::TYPE::BLOCK:
			{
				for (auto& hotbarItem : this->hotbar) {
					if (hotbarItem.has_value() &&
						hotbarItem.value().get()->getType() == INVENTORYITEM::TYPE::BLOCK &&
						hotbarItem.value().getAs<InventoryBlock>()->getBlock() == item.getAs<InventoryBlock>()->getBlock()
						) {
						if (hotbarItem.value().getAs<InventoryBlock>()->incrementCount(item.getAs<InventoryBlock>()->getCount())) {
							return true;
						}
					}
				}

				for (auto& inventoryItem : this->items) {
					if (inventoryItem.get()->getType() == INVENTORYITEM::TYPE::BLOCK &&
						item.getAs<InventoryBlock>()->getBlock() == inventoryItem.getAs<InventoryBlock>()->getBlock()) {
						if (inventoryItem.getAs<InventoryBlock>()->incrementCount(item.getAs<InventoryBlock>()->getCount())) {
							return true;
						}
					}
				}

				this->items.push_back(std::move(item));
				return true;
				break;
			}
		default:
			break;
	}
	return false;
}

bool Inventory::addItemCursor(UniqueReference<InventoryItem, InventoryItem>& item) {
	if (this->cursor.has_value()) {
		if (!this->addItem(this->cursor.value())) {
			return false;
		}
	}
	this->cursor = std::nullopt;
	this->cursor = std::move(item);
	return true;
}

bool Inventory::addItemCursor(UniqueReference<Activity, Activity>& activity) {
	UniqueReference<InventoryItem, InventoryItem> res = Locator<ReferenceManager<InventoryItem>>::ref().makeUniqueRef<InventoryActivity>(std::move(activity));

	if (this->addItemCursor(res)) {
		return true;
	}
	else {
		activity = std::move(res.getAs<InventoryActivity>()->extract());
		return false;
	}
}

bool Inventory::hasSpace() const {
	return true;
}

void Inventory::save(Saver& saver) {
	saver.store(this->items.size());
	for (auto& item : this->items) {
		saver.store(item);
	}

	bool hasCursor = this->cursor.has_value();
	saver.store(hasCursor);
	if (hasCursor) {
		saver.store(this->cursor.value());
	}

	saver.store(this->hotbar.size());
	for (auto& item : hotbar) {
		saver.store(item.has_value());
		if (item.has_value()) {
			saver.store(item.value());
		}
	}
}

void Inventory::load(Loader& loader) {
	size_t size;
	loader.retrieve(size);
	this->items.clear();
	this->items.resize(size);
	for (size_t i = 0; i < size; i++) {
		loader.retrieve(this->items[i]);
	}

	bool hasCursor;
	loader.retrieve(hasCursor);
	if (hasCursor) {
		this->cursor = UniqueReference<InventoryItem, InventoryItem>();
		loader.retrieve(cursor.value());
	}
	else {
		this->cursor = std::nullopt;
	}

	this->hotbar.clear();
	loader.retrieve(size);
	this->hotbar.resize(size);
	for (size_t i = 0; i < size; i++) {
		bool hasValue;
		loader.retrieve(hasValue);
		if (hasValue) {
			this->hotbar[i] = UniqueReference<InventoryItem, InventoryItem>();
			loader.retrieve(this->hotbar[i].value());
		}
		else {
			this->hotbar[i] = std::nullopt;
		}
	}
}
