#include "common.h"

#include "Shape.h"
#include "DataFront.h"
#include "BlockIDTextures.h"

void loadShapes() {

	auto set = [](SHAPE::TYPE type, std::string name)
	{
		DataFront<ShapeData>::data[type].texture = Locator<BlockIDTextures>::ref().getBlockTextureID(name + ".dds");
		DataFront<ShapeData>::names[type] = name;
		DataFront<ShapeData>::nameMap[name] = type;
	};

	set(SHAPE::TYPE::BLOCK, "block");
	set(SHAPE::TYPE::PISTON_BODY, "piston_body_stencil");
	set(SHAPE::TYPE::PISTON_HEAD, "piston_stencil");
	set(SHAPE::TYPE::SHAFT, "shaft_stencil");
	set(SHAPE::TYPE::CRANE_BASE_BLOCK, "crane_base_block_stencil");
	set(SHAPE::TYPE::CRANE_HEAD_BLOCK, "crane_head_block_stencil");
	set(SHAPE::TYPE::LUA, "lua_stencil");
	set(SHAPE::TYPE::GRABBER, "grabber_stencil");
	set(SHAPE::TYPE::READER, "reader_stencil");
	set(SHAPE::TYPE::DETECTOR, "detector_stencil");
	set(SHAPE::TYPE::INCINERATOR, "incinerator_stencil");
	set(SHAPE::TYPE::FORWARDER, "forwarder_stencil");
 }

