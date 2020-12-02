#include "common.h"

#include "UIOConstructActivityBuilder.h"

#include "UIOConstructer2.h"
#include "UIOSizeType.h"
#include "Enums.h"
#include "DataFront.h"
#include "Shape.h"
#include "UIODropDownList.h"
#include "Inventory.h"
#include "Log.h"

void UIO2::constructActivityBuilder() {
	const std::vector<std::pair<SHAPE::TYPE, int32_t>> shapes = {
		   {SHAPE::TYPE::PISTON_BODY, 1},
		   {SHAPE::TYPE::PISTON_HEAD, 1},
		   {SHAPE::TYPE::SHAFT, 2},
	};
	std::vector<UIODropDownList<ShapedBlock>*> selections;

	UIO2::startList(UIO::DIR::DOWN);

	for (auto [type, count] : shapes) {
		UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
		UIO2::startGrid(2, 1);
		UIO2::text(std::to_string(count) + "x " + DataFront<ShapeData>(type).getName());

		std::vector<ShapedBlock> items;

		for (auto const& item : Locator<Inventory>::ref().getItems()) {
			if (item.get()->getType() == INVENTORYITEM::TYPE::BLOCK) {
				auto block = static_cast<InventoryBlock*>(item.get());
				if (block->getBlock().getShapeData() == type) {
					items.push_back(block->getBlock());
				}
			}
		}

		auto list = UIO2::makeEnd<UIODropDownList<ShapedBlock>>([](ShapedBlock e)
		{
			return e.getBlock().name;
		});

		selections.push_back(list);

		list->setList(items);

		UIO2::endGrid();
	}

	auto button = UIO2::textButton("Spawn Activity");

	button->setOnRelease([shapes, selections](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		for (auto selection : selections) {
			if (!selection->getSelected().has_value()) {
				return BIND::RESULT::CONTINUE;
			}
		}

		std::vector<std::pair<ShapedBlock, int32_t>> collection;

		assert(shapes.size() == selections.size());

		for (int32_t i = 0; i < selections.size(); i++) {
			auto const& [shape, count] = shapes[i];
			auto const& block = selections[i]->getSelected().value();

			collection.push_back({ *block, count });
		}


		if (params.player.getInventory().extract(collection)) {
			Locator<Log>::ref().putLine("ActivityBuilder: Successfully extracted blocks from inventory");
		}
		else {
			Locator<Log>::ref().putLine("ActivityBuilder: Failed to extract blocks from inventory");
		}

		return BIND::RESULT::CONTINUE;
	});

	UIO2::endList();
}
