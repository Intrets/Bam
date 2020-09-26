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
}

