#include "common.h"

#include "InventoryItem.h"
#include "GameState.h"
#include "WorldBlock.h"
#include "RenderInfo.h"
#include "Colors.h"

bool InventoryBlock::place(GameState& gameState, glm::ivec2 pos) {
	if (gameState.staticWorld.setBlock(pos, this->block)) {
		this->count--;
	}
	return this->count == 0;
}

bool InventoryBlock::canPlace(GameState& gameState, glm::ivec2 pos) {
	return !gameState.staticWorld.getBlockRef(pos).isOccupied();
}

void InventoryBlock::rotate(ACTIVITY::ROT rot) {
	this->block.rotate(rot);
}

void InventoryBlock::setOrientation(ACTIVITY::DIR dir) {
	this->block.setOrientation(dir);
}

InventoryBlock::InventoryBlock(Handle self, ShapedBlock b, int32_t c) : block(b), count(c) {
	assert(c > 0);
	this->selfHandle = self;
}

void InventoryBlock::addWorldRenderInfo(GameState& gameState, RenderInfo& renderInfo, glm::ivec2 pos) {
	renderInfo.staticWorldRenderInfo.addBlockFront(pos, this->block);
	glm::vec4 color;

	if (this->canPlace(gameState, pos)) {
		color = COLORS::GR::HOVER;
	}
	else {
		color = COLORS::GR::BLOCKED;
	}
	renderInfo.selectionRenderInfo.addBox(pos, pos + glm::ivec2(1, 1), color);
}

void InventoryBlock::save(Saver& saver) {
	this->InventoryItem::save(saver);
	saver.store(this->count);
	this->block.save(saver);
}

void InventoryBlock::load(Loader& loader) {
	this->InventoryItem::load(loader);
	loader.retrieve(this->count);
	this->block.load(loader);
}

ShapedBlock const& InventoryBlock::getBlock() const {
	return this->block;
}

bool InventoryBlock::incrementCount(int32_t c) {
	assert(c >= 0);
	this->count += c;
	return true;
}

int32_t InventoryBlock::getCount() {
	return this->count;
}

std::string InventoryBlock::getName() {
	return std::to_string(this->count) + "x " + this->block.getString();
}

INVENTORYITEM::TYPE InventoryBlock::getType() {
	return INVENTORYITEM::TYPE::BLOCK;
}

Activity* InventoryActivity::getActivityPtr() {
	return this->activity.get();
}

InventoryActivity::InventoryActivity(Handle self, UniqueReference<Activity, Activity> a) {
	this->selfHandle = self;
	this->activity = std::move(a);
}

std::string InventoryActivity::getName() {
	return "Activity: " + this->activity.get()->getTypeName();
}

INVENTORYITEM::TYPE InventoryActivity::getType() {
	return INVENTORYITEM::TYPE::ACTIVITY;
}

bool InventoryActivity::place(GameState& gameState, glm::ivec2 pos) {
	auto a = this->activity.get();
	a->forceMoveOriginUp(pos - a->getOrigin());
	if (!a->canFillTracesUp(gameState)) {
		return false;
	}
	else {
		a->fillTracesUpForced(gameState);
		return true;
	}
}

bool InventoryActivity::canPlace(GameState& gameState, glm::ivec2 pos) {
	auto a = this->activity.get();
	a->forceMoveOriginUp(pos - a->getOrigin());

	return a->canFillTracesLocal(gameState);
}

void InventoryActivity::rotate(ACTIVITY::ROT rot) {
	this->activity.get()->rotateForcedUp(glm::ivec2(0, 0), rot);
}

void InventoryActivity::setOrientation(ACTIVITY::DIR dir) {
}

void InventoryActivity::addWorldRenderInfo(GameState& gameState, RenderInfo& renderInfo, glm::ivec2 pos) {
	auto a = this->activity.get();
	a->forceMoveOriginUp(pos - a->getOrigin());

	for (auto member : a->getTreeMembers()) {
		member->appendStaticRenderInfo(gameState, renderInfo.staticWorldRenderInfo);
	}

	glm::vec4 color;

	if (this->canPlace(gameState, pos)) {
		color = COLORS::GR::HOVER;
	}
	else {
		color = COLORS::GR::BLOCKED;
	}

	a->appendSelectionInfo(gameState, renderInfo, color);
}

void InventoryActivity::save(Saver& saver) {
	this->InventoryItem::save(saver);
	saver.store(this->activity.handle);
}

void InventoryActivity::load(Loader& loader) {
	this->InventoryItem::load(loader);
	loader.retrieve(this->activity.handle);
}

void InventoryItem::save(Saver& saver) {
	saver.store(this->selfHandle);
}

void InventoryItem::load(Loader& loader) {
	loader.retrieve(this->selfHandle);
}
