#include "common.h"

#include "InventoryItem.h"
#include "GameState.h"
#include "WorldBlock.h"
#include "RenderInfo.h"
#include "Colors.h"

bool InventoryBlock::place(GameState& gameState, glm::ivec2 pos) {
	return gameState.staticWorld.setBlock(pos, this->block);
}

bool InventoryBlock::canPlace(GameState& gameState, glm::ivec2 pos) {
	return gameState.staticWorld.getBlockRef(pos).isOccupied();
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

bool InventoryActivity::place(GameState& gameState, glm::ivec2 pos) {
	auto a = this->activity.get();
	a->forceMoveOriginUp(pos - a->getOrigin());
	if (a->canFillTracesLocal(gameState)) {
		return false;
	}
	else {
		a->fillTracesLocalForced(gameState);
		return true;
	}
}

bool InventoryActivity::canPlace(GameState& gameState, glm::ivec2 pos) {
	auto a = this->activity.get();
	a->forceMoveOriginUp(pos - a->getOrigin());

	return a->canFillTracesLocal(gameState);
}

void InventoryActivity::addWorldRenderInfo(GameState& gameState, RenderInfo& renderInfo, glm::ivec2 pos) {
	auto a = this->activity.get();
	a->forceMoveOriginUp(pos - a->getOrigin());

	a->appendStaticRenderInfo(gameState, renderInfo.staticWorldRenderInfo);

	glm::vec4 color;

	if (this->canPlace(gameState, pos)) {
		color = COLORS::GR::HOVER;
	}
	else {
		color = COLORS::GR::BLOCKED;
	}

	a->appendSelectionInfo(gameState, renderInfo, color);
}

