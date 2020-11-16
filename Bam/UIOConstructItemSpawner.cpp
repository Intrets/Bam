#include "common.h"

#include "UIOConstructItemSpawner.h"

#include "UIOList.h"
#include "UIOConstructer.h"
#include "Enums.h"
#include "UIOTextConstructers.h"
#include "Inventory.h"
#include "ActivitySpawner.h"
#include "UIOCallBackParams.h"
#include "DataFront.h"
#include "Block.h"
#include "Shape.h"
#include "UIOListSelection.h"
#include "UIOTextConstructers.h"

UIOConstructer<UIOList> CONSTRUCTER::constructItemSpawner() {
	UIOList* listPtr;
	auto list = UIOConstructer<UIOList>::makeConstructer(UIO::DIR::DOWN).setPtr(listPtr);

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
		listPtr->addElement(
			TextConstructer::constructSingleLineDisplayText(ACTIVITY::GET_TYPE_NAME(type))
			.alignCenter()
			.button()
			.onRelease(
				[type](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			if (auto maybeItem = ACTIVITYSPAWNER::spawn(params.gameState, { 0,0 }, type)) {
				auto& item = maybeItem.value();
				UniqueReference<InventoryItem, InventoryItem> inventoryItem = Locator<ReferenceManager<InventoryItem>>::ref().makeUniqueRef<InventoryActivity>(std::move(item));
				Locator<Inventory>::ref().addItemCursor(inventoryItem);
			}
			else {
				Locator<Log>::ref().putLine("Spawning of activity " + ACTIVITY::GET_TYPE_NAME(type) + " failed or not implemented");
			}

			return BIND::RESULT::CONTINUE;
		})
			.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
			.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
			.get()
			);
	}

	UIOListSelection<DataFront<BlockData>>* blockSelectionPtr;
	auto blockSelection =
		UIOConstructer<UIOListSelection<DataFront<BlockData>>>::makeConstructer(
			[](DataFront<BlockData> const& e)
	{
		return e.getName();
	})
		.addBind(
			[](UIOListSelection<DataFront<BlockData>>* l)
	{
		auto blocks = DataFront<BlockData>::listAll();
		l->setList(blocks);
	})
		.setPtr(blockSelectionPtr);

	UIOListSelection<DataFront<ShapeData>>* shapeSelectionPtr;
	auto shapeSelection =
		UIOConstructer<UIOListSelection<DataFront<ShapeData>>>::makeConstructer(
			[](DataFront<ShapeData> const& e)
	{
		return e.getName();
	})
		.addBind(
			[](UIOListSelection<DataFront<ShapeData>>* l)
	{
		auto shapes = DataFront<ShapeData>::listAll();
		l->setList(shapes);
	})
		.setPtr(shapeSelectionPtr);

	UIOList* c;
	listPtr->addElement(
		UIOConstructer<UIOList>::makeConstructer(UIO::DIR::UP)
		.setPtr(c)
		.get()
	);

	c->addElement(
		TextConstructer::constructSingleLineDisplayText("Spawn Block")
		.button()
		.onPress([shapeSelectionPtr, blockSelectionPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto block = blockSelectionPtr->getSelected();
		auto shape = shapeSelectionPtr->getSelected();

		if (block.has_value() && shape.has_value()) {
			ShapedBlock shapedBlock{ *block.value(), *shape.value(), ACTIVITY::DIR::RIGHT };

			UniqueReference<InventoryItem, InventoryItem> inventoryItem = Locator<ReferenceManager<InventoryItem>>::ref().makeUniqueRef<InventoryBlock>(shapedBlock, 1);
			Locator<Inventory>::ref().addItemCursor(inventoryItem);
		}
		return BIND::RESULT::CONTINUE;
	})
		.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
		.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
		.get()
		);

	UIOList* shapedBlockSelectionCombo;

	c->addElement(
		UIOConstructer<UIOList>::makeConstructer(UIO::DIR::RIGHT)
		.setPtr(shapedBlockSelectionCombo)
		.get()
	);

	shapedBlockSelectionCombo->addElement(
		blockSelection
		.constrainWidth({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.5f })
		.get()
	);

	shapedBlockSelectionCombo->addElement(
		shapeSelection
		.get()
	);

	return list;
}
