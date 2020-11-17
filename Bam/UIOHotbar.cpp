#include "common.h"

#include "UIOHotbar.h"
#include "UIOTextConstructers.h"
#include "UIOConstructer.h"
#include "UIOTextDisplay.h"
#include "Inventory.h"
#include "InventoryItem.h"

Inventory& UIOHotbar::getInventory() {
	return Locator<Inventory>::ref();
}

UIOHotbar::UIOHotbar(Handle self) : UIOGrid(self, glm::ivec2(10, 1)) {
	this->icons.reserve(10);

	for (int32_t i = 0; i < 10; i++) {
		UIOTextDisplay* ptr;
		this->addElement(
			TextConstructer::constructSingleLineDisplayText("")
			.setPtr(ptr)
			.alignCenter()
			.button()
			.onPress([i, this](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			static_cast<UIOHotbar*>(self_)->getInventory().clickHotbar(i);
			return BIND::RESULT::CONTINUE;
		})
			.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
			.get()
			);
		this->icons.push_back(ptr);
	}
}

int32_t UIOHotbar::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	int32_t i = 0;
	for (auto const& item : this->getInventory().getHotbar()) {
		if (i >= this->icons.size()) {
			break;
		}
		if (item.has_value()) {
			this->icons[i]->setText(item.value().get()->getName());
		}
		else {
			this->icons[i]->setText("");
		}
		i++;
	}

	return this->UIOGrid::addRenderInfo(gameState, renderInfo, depth);
}

