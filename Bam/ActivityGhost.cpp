#include "common.h"

#include <algorithm>

#include "ActivityGhost.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "Block.h"
#include "WorldBlock.h"
#include "Shape.h"
#include "Colors.h"
#include "Inventory.h"
#include "UIState.h"

bool ActivityGhost::validate(GameState& gameState) {
	return false;
}

void ActivityGhost::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, UIState& uiState, Inventory& inventory) {
	auto cursorPos = glm::ivec2(glm::floor(uiState.getCursorPositionWorld()));
	for (int32_t i = 0; i < this->size.x; i++) {
		for (int32_t j = 0; j < this->size.y; j++) {
			if (auto const& maybeShape = this->shapes[i][j]) {
				auto const pos = this->origin + glm::ivec2(i, j);
				auto& shape = maybeShape.value();

				auto const* worldBlock = &gameState.staticWorld.getBlockConstRef(pos).getShapedBlock();
				if (cursorPos.x == i && cursorPos.y == j && worldBlock->getBlockData().getName() == "air") {
					if (auto const& cursor = inventory.getCursor()) {
						if (cursor.value().get()->getType() == INVENTORYITEM::TYPE::BLOCK) {
							worldBlock = &cursor.value().getAs<InventoryBlock>()->getBlock();
						}
					}
				}

				if (worldBlock->getShapeData().getID() == shape.type &&
					shape.validDirection(worldBlock->getRotation())) {
					renderInfo.selectionRenderInfo.addBox(pos, pos + 1, COLORS::rgba(0, 255, 0, 0.5f));
				}
				else if (worldBlock->getBlockData().getName() == "air") {
					ShapedBlock shapedBlock{ "iron", shape.type, shape.toDir() };
					renderInfo.staticWorldRenderInfo.addBlockFront(pos, shapedBlock);
					renderInfo.selectionRenderInfo.addBox(pos, pos + 1, COLORS::rgba(100, 100, 100, 0.5f));
				}
				else {
					renderInfo.selectionRenderInfo.addBox(pos, pos + 1, COLORS::rgba(255, 0, 0, 0.5f));
				}
			}
		}
	}
}

ActivityGhost::ActivityGhost() :
	size(5, 2),
	shapes(5, std::vector<std::optional<ShapeInfo>>{2, std::nullopt}) {

	shapes[0][0] = { SHAPE::TYPE::CRANE_BASE_BLOCK, {} };
	shapes[4][0] = { SHAPE::TYPE::CRANE_BASE_BLOCK, {} };

	for (size_t i = 0; i < 5; i++) {
		shapes[i][1] = { SHAPE::TYPE::SHAFT, {ACTIVITY::DIR::RIGHT, ACTIVITY::DIR::LEFT} };
	}

	shapes[2][1] = { SHAPE::TYPE::CRANE_HEAD_BLOCK, {} };
}

bool ShapeInfo::validDirection(ACTIVITY::DIR dir) const {
	if (this->dirs.size() == 0) {
		return true;
	}
	else {
		return std::find(this->dirs.begin(), this->dirs.end(), dir) != this->dirs.end();
	}
}

ACTIVITY::DIR ShapeInfo::toDir() const {
	if (this->dirs.size() == 0) {
		return ACTIVITY::DIR::RIGHT;
	}
	return dirs[0];
}
