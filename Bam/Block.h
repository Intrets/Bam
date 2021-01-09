#pragma once

#include <string>

#include "Shape.h"
#include "DataFront.h"

#include "Enums.h"

class ActivityIgnoringGroup;
class Loader;
class Saver;

#define MAXBLOCKS 1000

struct BlockData
{
	std::string name = "air";
	bool solid = false;
	int32_t texture = 0;

	int32_t min = 10;
	int32_t max = 15;

	int32_t average = 12;

	int32_t getSmallRand(GameState& gameState) const;
	int32_t getVal() const;
};

class ShapedBlock
{
private:
	DataFront<BlockData> block;
	DataFront<ShapeData> shape;

	ACTIVITY::DIR rotation = ACTIVITY::DIR::RIGHT;

	friend class WorldBlock;

public:
	BlockData const& getBlock() const;
	ShapeData const& getShape() const;

	DataFront<BlockData> const& getBlockData() const;
	DataFront<ShapeData> const& getShapeData() const;

	int32_t getTexture() const;
	int32_t getStencil() const;
	ACTIVITY::DIR getRotation() const;

	std::string getString();

	void rotate(ACTIVITY::ROT rot);
	void setOrientation(ACTIVITY::DIR dir);

	bool isSolid() const;
	bool isNonAir() const;

	bool operator==(ShapedBlock const& other) const;

	ShapedBlock(std::string name);
	ShapedBlock(int32_t blockID, int32_t shapeID, ACTIVITY::DIR rot);
	ShapedBlock(std::string const& block, std::string const& shape, ACTIVITY::DIR rot);
	ShapedBlock(std::string const& block, SHAPE::TYPE shapeID, ACTIVITY::DIR rot);
	ShapedBlock(DataFront<BlockData> blockID, ACTIVITY::DIR rot);
	ShapedBlock(DataFront<BlockData> blockID, DataFront<ShapeData> shapeID, ACTIVITY::DIR rot);
	ShapedBlock(std::string const& block, DataFront<ShapeData> shapeID, ACTIVITY::DIR rot);
	ShapedBlock() = default;
	~ShapedBlock() = default;

	bool load(Loader& loader);
	bool save(Saver& saver);
};

void loadBlocks(bool graphics);
