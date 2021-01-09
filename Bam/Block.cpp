#include "common.h"

#include "Block.h"

#include <fstream>

#include "ActivityIgnoringGroup.h"
#include "Loader.h"
#include "Saver.h"
#include "StringHelpers.h"
#include "BlockIDTextures.h"
#include "GameState.h"
#include "Locator.h"
#include "PathManager.h"

void loadBlocks() {
	std::ifstream file;
	Locator<PathManager>::ref().openBlockData(file);

	std::string line;
	int32_t i = 0;
	while (!file.eof()) {
		std::unordered_map<std::string, std::string> pairs;
		for (int32_t j = 0; j < 5; j++) {
			std::vector<std::string> buffer;
			std::getline(file, line);
			if (line.size() == 0) {
				return;
			}
			split(1, line, buffer, '=');
			pairs[buffer[0]] = buffer[1];
		}
		std::getline(file, line);
		assert(line == "");

		DataFront<BlockData>::data[i].name = pairs["name"];
		DataFront<BlockData>::names[i] = pairs["name"];

		DataFront<BlockData>::data[i].solid = pairs["solid"] == "true";
		DataFront<BlockData>::data[i].texture = Locator<BlockIDTextures>::ref().getBlockTextureID(pairs["texture"]);

		DataFront<BlockData>::data[i].min = std::stoi(pairs["min"]);
		DataFront<BlockData>::data[i].max = std::stoi(pairs["max"]);
		DataFront<BlockData>::data[i].average = (DataFront<BlockData>::data[i].max + DataFront<BlockData>::data[i].min) / 2;

		std::vector<std::string> elementPairs;

		DataFront<BlockData>::nameMap[pairs["name"]] = i;

		i++;
		DataFront<BlockData>::size = i;
	}
}

int32_t BlockData::getSmallRand(GameState& gameState) const {
	return gameState.smallRandom.randRange(this->min, this->max);
}

int32_t BlockData::getVal() const {
	return this->average;
}

BlockData const& ShapedBlock::getBlock() const {
	return this->block.getData();
}

ShapeData const& ShapedBlock::getShape() const {
	return this->shape.getData();
}

DataFront<BlockData> const& ShapedBlock::getBlockData() const {
	return this->block;
}

DataFront<ShapeData> const& ShapedBlock::getShapeData() const {
	return this->shape;
}

int32_t ShapedBlock::getTexture() const {
	return this->block.getData().texture;
}

int32_t ShapedBlock::getStencil() const {
	return this->shape.getData().texture;
}

ACTIVITY::DIR ShapedBlock::getRotation() const {
	return this->rotation;
}

std::string ShapedBlock::getString() {
	return this->block.getName() + " " + this->shape.getName();
}

void ShapedBlock::rotate(ACTIVITY::ROT rot) {
	this->rotation = ACTIVITY::ROTATE(rot, this->rotation);
}

void ShapedBlock::setOrientation(ACTIVITY::DIR dir) {
	this->rotation = dir;
}

bool ShapedBlock::isSolid() const {
	return this->block.getData().solid;
}

bool ShapedBlock::isNonAir() const {
	return this->block.getID() != 0;
}

bool ShapedBlock::operator==(ShapedBlock const& other) const {
	return this->block.getID() == other.block.getID() && this->shape.getID() == other.shape.getID();
}

ShapedBlock::ShapedBlock(std::string name) : block(name) {
}

ShapedBlock::ShapedBlock(int32_t blockID, int32_t shapeID, ACTIVITY::DIR dir) : block(blockID), shape(shapeID), rotation(dir) {
}

ShapedBlock::ShapedBlock(std::string const& block, std::string const& shape, ACTIVITY::DIR dir) : block(block), shape(shape), rotation(dir) {
}

ShapedBlock::ShapedBlock(std::string const& block, SHAPE::TYPE shapeID, ACTIVITY::DIR rot) : block(block), shape(shapeID), rotation(rot) {
}

ShapedBlock::ShapedBlock(DataFront<BlockData> blockID, ACTIVITY::DIR rot) : block(blockID), rotation(rot) {
}

ShapedBlock::ShapedBlock(DataFront<BlockData> blockID, DataFront<ShapeData> shapeID, ACTIVITY::DIR rot) : block(blockID), shape(shapeID), rotation(rot) {
}

ShapedBlock::ShapedBlock(std::string const& block, DataFront<ShapeData> shapeID, ACTIVITY::DIR rot) : block(block), shape(shapeID), rotation(rot) {
}

bool ShapedBlock::load(Loader& loader) {
	this->block.load(loader);
	this->shape.load(loader);
	loader.retrieve(this->rotation);
	return true;
}

bool ShapedBlock::save(Saver& saver) {
	this->block.save(saver);
	this->shape.save(saver);
	saver.store(this->rotation);
	return true;
}

