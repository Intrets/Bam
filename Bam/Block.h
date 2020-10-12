#pragma once

#include "Shape.h"
#include "DataFront.h"

#include "Enums.h"

class ActivityIgnoringGroup;
class Loader;
class Saver;

#define MAXBLOCKS 1000

struct Element
{
	ELEMENT::TYPE type;
	int32_t quantity;

	Element() = default;
	Element(ELEMENT::TYPE t, int32_t q);
	Element(Loader& loader);

	void save(Saver& saver);
	void load(Loader& loader);
};

struct Material
{
	std::vector<Element> elements;

	int32_t min = 10;
	int32_t max = 15;

	int32_t average = 12;

	int32_t getSmallRand(GameState& gameState) const;
	int32_t getVal() const;

	Material() = default;
	Material(Loader& loader);

	void save(Saver& saver);
	void load(Loader& loader);
};

struct BlockData
{
	std::string name = "air";
	bool solid = false;
	int32_t texture = 0;

	Material material;
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
	ShapedBlock(std::string block, std::string shape, ACTIVITY::DIR rot);
	ShapedBlock(std::string block, SHAPE::TYPE shapeID, ACTIVITY::DIR rot);
	ShapedBlock() = default;
	~ShapedBlock() = default;

	bool load(Loader& loader);
	bool save(Saver& saver);
};

void loadBlocks();
