#include "common.h"

#include "UIOHotbar.h"
#include "UIOTextDisplay.h"
#include "Inventory.h"
#include "InventoryItem.h"
#include "UIOConstructer2.h"
#include "UIOButton.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "PlayerState.h"

UIOHotbar::UIOHotbar(Handle self) : UIOGrid(self, glm::ivec2(10, 1)) {
	this->icons.reserve(10);

	for (int32_t i = 0; i < 10; i++) {
		UIO2::Global::push();

		auto [button, text] = UIO2::textButton2(std::to_string(i));

		this->icons.push_back(text.get());
		button.get()->setOnRelease([i, this](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
		{
			playerState.getPlayer().getInventory().clickHotbar(i);
			return BIND::RESULT::CONTINUE;
		});

		this->addElement(UIO2::Global::pop());
	}

	const std::pair<int32_t, CONTROL::KEY> maps[] = {
		{0, CONTROL::KEY::TOOL_1},
		{1, CONTROL::KEY::TOOL_2},
		{2, CONTROL::KEY::TOOL_3},
		{3, CONTROL::KEY::TOOL_4},
		{4, CONTROL::KEY::TOOL_5},
		{5, CONTROL::KEY::TOOL_6},
		{6, CONTROL::KEY::TOOL_7},
		{7, CONTROL::KEY::TOOL_8},
		{8, CONTROL::KEY::TOOL_9},
		{9, CONTROL::KEY::TOOL_0},
	};

	for (auto [i, key] : maps) {
		this->addGameWorldBind({ key }, [i = i](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
		{
			playerState.getPlayer().getInventory().clickHotbar(i);
			return BIND::RESULT::CONTINUE;
		});
	}
}

int32_t UIOHotbar::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (auto player = gameState.getPlayer(renderInfo.playerIndex)) {
		int32_t i = 0;
		for (auto const& item : player.value()->getInventory().getHotbar()) {
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
	else {
		Locator<Log>::ref().putLine("UIOHotbar: did not find player specified in renderInfo");
		return depth;
	}
}

