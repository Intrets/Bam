#include "common.h"

#include "UIOInventory.h"
#include "Inventory.h"
#include "InventoryItem.h"
#include "UIOTextDisplay.h"
#include "UIOConstructer2.h"
#include "UIOButton.h"

Inventory& UIOInventory::getInventory() {
	return Locator<Inventory>::ref();
}

UIOInventory::UIOInventory(Handle self) : UIOGrid(self, glm::ivec2(4, 4)) {
	this->icons.reserve(16);

	for (int32_t i = 0; i < 16; i++) {
		UIO2::Global::push();

		auto [button, text] = UIO2::textButton2(std::to_string(i));

		this->icons.push_back(text.get());
		button.get()->setOnRelease([i, this](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			static_cast<UIOInventory*>(self_)->getInventory().clickInventory(i);
			return BIND::RESULT::CONTINUE;
		});

		this->addElement(UIO2::Global::pop());
	}
}

int32_t UIOInventory::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	int32_t i = 0;
	for (auto const& item : this->getInventory().getItems()) {
		if (i >= this->icons.size()) {
			break;
		}
		this->icons[i]->setText(item.get()->getName());
		i++;
	}
	for (; i < this->icons.size(); i++) {
		this->icons[i]->setText("");
	}

	return this->UIOGrid::addRenderInfo(gameState, renderInfo, depth);
}

