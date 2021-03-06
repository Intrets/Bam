#include "common.h"

#include "UIOConstructItemSpawner.h"

#include "UIOList.h"
#include "Enums.h"
#include "Inventory.h"
#include "ActivitySpawner.h"
#include "PlayerState.h"
#include "DataFront.h"
#include "Block.h"
#include "Shape.h"
#include "UIOListSelection.h"
#include "UIOConstructer2.h"
#include "UIOSizeType.h"
#include "UIOButton.h"
#include "Locator.h"
#include "Log.h"

UIOList* UIO2::constructItemSpawner() {
	UIO2::startList(UIO::DIR::DOWN);

	const ACTIVITY::TYPE activities[] = {
		ACTIVITY::TYPE::MOVER,
		ACTIVITY::TYPE::PISTON,
		ACTIVITY::TYPE::BREAKER,
		ACTIVITY::TYPE::GRABBER,
		ACTIVITY::TYPE::RAILCRANE,
		ACTIVITY::TYPE::LUA,
		ACTIVITY::TYPE::READER,
		ACTIVITY::TYPE::DETECTOR,
		ACTIVITY::TYPE::INCINERATOR,
		ACTIVITY::TYPE::FORWARDER,
	};

	for (auto type : activities) {
		UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
		UIO2::textButton(ACTIVITY::GET_TYPE_NAME(type)).get()->setOnRelease(
			[type](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
		{
			if (auto maybeItem = ACTIVITYSPAWNER::spawn(playerState.gameState, { 0,0 }, type)) {
				auto& item = maybeItem.value();
				UniqueReference<InventoryItem, InventoryItem> inventoryItem = playerState.gameState.getInventoryItemManager().makeUniqueRef<InventoryActivity>(std::move(item));
				playerState.getPlayer().getInventory().addItemCursor(inventoryItem);
			}
			else {
				Locator<Log>::ref().putLine("Spawning of activity " + ACTIVITY::GET_TYPE_NAME(type) + " failed or not implemented");
			}

			return BIND::RESULT::CONTINUE;
		});
	}

	UIO2::startList(UIO::DIR::DOWN_REVERSE);

	UIO2::startGrid(2, 1);

	auto blocksList = DataFront<BlockData>::listAll();

	UIO2::pad({ UIO::SIZETYPE::PX, 1 });
	auto blockSelection = UIO2::makeEnd<UIOListSelection<DataFront<BlockData>>>(
		[](DataFront<BlockData> const& e)
	{
		return e.getName();
	});
	blockSelection.get()->setList(blocksList);

	auto shapesList = DataFront<ShapeData>::listAll();

	UIO2::pad({ UIO::SIZETYPE::PX, 1 });
	auto shapeSelection = UIO2::makeEnd<UIOListSelection<DataFront<ShapeData>>>(
		[](DataFront<ShapeData> const& e)
	{
		return e.getName();
	});
	shapeSelection.get()->setList(shapesList);

	UIO2::endGrid();

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	UIO2::textButton("Spawn Block").get()->setOnRelease([shapeSelection = shapeSelection.get(), blockSelection = blockSelection.get()](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto block = blockSelection->getSelected();
		auto shape = shapeSelection->getSelected();

		if (block.has_value() && shape.has_value()) {
			ShapedBlock shapedBlock{ *block.value(), *shape.value(), ACTIVITY::DIR::RIGHT };

			UniqueReference<InventoryItem, InventoryItem> inventoryItem = playerState.gameState.getInventoryItemManager().makeUniqueRef<InventoryBlock>(shapedBlock, 1);
			playerState.getPlayer().getInventory().addItemCursor(inventoryItem);
		}
		return BIND::RESULT::CONTINUE;
	});

	UIO2::endList();
	UIO2::endList();
	return nullptr;
}
