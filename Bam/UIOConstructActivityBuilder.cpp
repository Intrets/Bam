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
#include "ActivitySpawner.h"
#include "Piston.h"
#include "UIOCallBackParams.h"

bool UIO2::constructActivityBuilder(ACTIVITY::TYPE activityType) {
	std::vector<std::pair<SHAPE::TYPE, int32_t>> shapes;

	switch (activityType) {
		case ACTIVITY::TYPE::ANCHOR:
			return false;
			break;
		case ACTIVITY::TYPE::MOVER:
			return false;
			break;
		case ACTIVITY::TYPE::PLATFORM:
			return false;
			break;
		case ACTIVITY::TYPE::PISTON:
			shapes = {
			   {SHAPE::TYPE::PISTON_BODY, 1},
			   {SHAPE::TYPE::PISTON_HEAD, 1},
			   {SHAPE::TYPE::SHAFT, 2},
			};
			break;
		case ACTIVITY::TYPE::SINGLEPLATFORM:
			return false;
			break;
		case ACTIVITY::TYPE::BREAKER:
			return false;
			break;
		case ACTIVITY::TYPE::GRABBER:
			shapes = {
				{SHAPE::TYPE::GRABBER, 1},
			};
			break;
		case ACTIVITY::TYPE::PLANT:
			return false;
			break;
		case ACTIVITY::TYPE::RAILCRANE:
			shapes = {
			   {SHAPE::TYPE::CRANE_BASE_BLOCK, 2},
			   {SHAPE::TYPE::CRANE_HEAD_BLOCK, 1},
			   {SHAPE::TYPE::SHAFT, 8},
			};
			break;
		case ACTIVITY::TYPE::LUA:
			shapes = {
				{SHAPE::TYPE::LUA, 1},
			};
			break;
		case ACTIVITY::TYPE::READER:
			shapes = {
				{SHAPE::TYPE::READER, 1},
			};
			break;
		case ACTIVITY::TYPE::DETECTOR:
			shapes = {
				{SHAPE::TYPE::DETECTOR, 1},
			};
			break;
		case ACTIVITY::TYPE::INCINERATOR:
			shapes = {
				{SHAPE::TYPE::INCINERATOR, 1},
			};
			break;
		case ACTIVITY::TYPE::FORWARDER:
			shapes = {
				{SHAPE::TYPE::FORWARDER, 1},
			};
			break;
		case ACTIVITY::TYPE::_MAX:
			assert(0);
			return false;
			break;
		default:
			assert(0);
			return false;
			break;
	}

	std::vector<UIODropDownList<ShapedBlock>*> selections;

	UIO2::startList(UIO::DIR::DOWN);

	for (auto [type, count] : shapes) {
		UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
		UIO2::startGrid(2, 1);
		UIO2::text(std::to_string(count) + "x " + DataFront<ShapeData>(type).getName());

		auto list = UIO2::makeEnd<UIODropDownList<ShapedBlock>>([](ShapedBlock e)
		{
			return e.getBlock().name;
		});

		selections.push_back(list.get());

		list.get()->setList([type = type](UIOCallBackParams& params)
		{
			std::vector<ShapedBlock> items;

			for (auto const& item : params.player.getInventory().getItems()) {
				if (item.get()->getType() == INVENTORYITEM::TYPE::BLOCK) {
					auto block = static_cast<InventoryBlock*>(item.get());
					if (block->getBlock().getShapeData() == type) {
						items.push_back(block->getBlock());
					}
				}
			}
			return items;
		});

		UIO2::endGrid();
	}

	auto button = UIO2::textButton("Spawn Activity");

	button.get()->setOnRelease([shapes, selections = std::move(selections), activityType](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (!params.player.getInventory().hasSpace()) {
			Locator<Log>::ref().putLine("No space in inventory");
			return BIND::RESULT::CONTINUE;
		}

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

			auto activity = ACTIVITYSPAWNER::spawn(params.gameState, { 0,0 }, activityType);

			assert(activity.has_value());

			auto success = params.player.getInventory().addItemCursor(activity.value());

			if (success) {
				Locator<Log>::ref().putLine("ActivityBuilder: Successfully placed spawned activity in inventory");
			}
			else {
				Locator<Log>::ref().putLine("ActivityBuilder: Failed to place spawned activity in inventory");
				assert(0);
			}
		}
		else {
			Locator<Log>::ref().putLine("ActivityBuilder: Failed to extract blocks from inventory");
		}

		return BIND::RESULT::CONTINUE;
	});

	UIO2::endList();

	return true;
}
