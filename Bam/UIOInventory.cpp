#include "common.h"

#include "UIOInventory.h"
#include "Inventory.h"
#include "InventoryItem.h"
#include "UIOTextDisplay.h"
#include "UIOConstructer.h"
#include "UIOTextConstructers.h"
#include "UIOBinds.h"

Inventory& UIOInventory::getInventory() {
	return Locator<Inventory>::ref();
}

UIOInventory::UIOInventory(Handle self) : UIOGrid(self, glm::ivec2(10, 10)) {
	this->icons.reserve(100);

	for (size_t i = 0; i < 100; i++) {
		UIOTextDisplay* ptr;
		this->addElement(
			TextConstructer::constructSingleLineDisplayText(std::to_string(i))
			.setPtr(ptr)
			.alignCenter()
			.button()
			.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
			.get()
			);
		this->icons.push_back(ptr);
	}
}

int32_t UIOInventory::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	int32_t i = 0;
	for (auto const& item : this->getInventory().getItems()) {
		if (i > this->icons.size()) {
			break;
		}
		this->icons[i]->setText(item.get()->getName());
		i++;
	}
	for (; i < this->icons.size(); i++) {
		this->icons[i]->setText("123");
	}

	return this->UIOGrid::addRenderInfo(gameState, renderInfo, depth);
}

