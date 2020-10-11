#include "common.h"

#include "UIOInventory.h"
#include "Inventory.h"
#include "InventoryItem.h"
#include "UIOTextDisplay.h"
#include "UIOConstructer.h"
#include "UIOTextConstructers.h"

Inventory& UIOInventory::getInventory() {
	return Locator<Inventory>::ref();
}

UIOInventory::UIOInventory(Handle self) : UIOGrid(self, glm::ivec2(4, 4)) {
	this->icons.reserve(16);

	for (int32_t i = 0; i < 16; i++) {
		UIOTextDisplay* ptr;
		this->addElement(
			TextConstructer::constructSingleLineDisplayText(std::to_string(i))
			.setPtr(ptr)
			.alignCenter()
			.button()
			.onPress([i, this](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			static_cast<UIOInventory*>(self_)->getInventory().clickInventory(i);
			return BIND::RESULT::CONTINUE;
		})
			.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
			.get()
			);
		this->icons.push_back(ptr);
	}

}

int32_t UIOInventory::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	int32_t i = 0;
	for (auto const& item : this->getInventory().getItems()) {
		if (i >= this->icons.size()) {
			break;
		}
		auto& t = this->getInventory();
		auto man = Locator<ReferenceManager<InventoryItem>>::get();
		auto man2 = Locator<ReferenceManager<Activity>>::get();
		this->icons[i]->setText(item.get()->getName());
		i++;
	}
	for (; i < this->icons.size(); i++) {
		this->icons[i]->setText("");
	}

	return this->UIOGrid::addRenderInfo(gameState, renderInfo, depth);
}

