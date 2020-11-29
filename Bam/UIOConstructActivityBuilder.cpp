#include "common.h"

#include "UIOConstructActivityBuilder.h"

#include "UIOConstructer2.h"
#include "UIOSizeType.h"
#include "Enums.h"
#include "DataFront.h"
#include "Shape.h"
#include "UIODropDownList.h"
#include "Inventory.h"

void UIO2::constructActivityBuilder() {
	const std::pair<SHAPE::TYPE, int32_t> shapes[] = {
		{SHAPE::TYPE::PISTON_BODY, 1},
		{SHAPE::TYPE::PISTON_HEAD, 1},
		{SHAPE::TYPE::SHAFT, 2},
	};

	UIO2::startList(UIO::DIR::DOWN);

	for (auto [type, count] : shapes) {
		UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
		UIO2::startGrid(2, 1);
		UIO2::text(std::to_string(count) + "x " + DataFront<ShapeData>(type).getName());

		std::vector<std::string> items;

		for (auto const& item : Locator<Inventory>::ref().getItems()) {
			if (item.get()->getType() == INVENTORYITEM::TYPE::BLOCK) {
				auto block = static_cast<InventoryBlock*>(item.get());
				items.push_back(block->getBlock().getShape().name);
			}
		}

		/*auto list = */UIO2::makeEnd<UIODropDownList<std::string>>([](auto e)
		{
			return e;
		});

		//list->setList(items);

		UIO2::endGrid();
	}

	UIO2::endList();
}
