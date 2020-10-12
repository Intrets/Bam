#include "common.h"
#include "Block.h"
#include "ActivityIgnoringGroup.h"

#include "Loader.h"
#include "Saver.h"

#include <fstream>
#include "StringHelpers.h"
#include "BlockIDTextures.h"
#include "GameState.h"

void loadBlocks() {
	std::ifstream file;
	Locator<PathManager>::ref().openBlockData(file);

	std::string line;
	int32_t i = 0;
	while (!file.eof()) {
		std::unordered_map<std::string, std::string> pairs;
		for (int32_t j = 0; j < 4; j++) {
			std::vector<std::string> buffer;
			std::getline(file, line);
			if (line.size() == 0) {
				return;
			}
			split(1, line, buffer, '=');
			pairs[buffer[0]] = buffer[1];
		}

		DataFront<BlockData>::data[i].name = pairs["name"];
		DataFront<BlockData>::names[i] = pairs["name"];
		DataFront<BlockData>::data[i].solid = pairs["solid"] == "true";
		DataFront<BlockData>::data[i].texture = Locator<BlockIDTextures>::ref().getBlockTextureID(pairs["texture"]);

		std::vector<std::string> elementPairs;

		split(0, pairs["elements"], elementPairs, ',');

		for (auto& elementPair : elementPairs) {
			std::vector<std::string> e;
			split(0, elementPair, e, ':');
			DataFront<BlockData>::data[i].material.elements.push_back({ ELEMENT::getType(e[0]), std::stoi(e[1].c_str()) });
		}

		DataFront<BlockData>::nameMap[pairs["name"]] = i;

		i++;
	}
}

int32_t Material::getSmallRand(GameState& gameState) const {
	return gameState.smallRandom.randRange(this->min, this->max);
}

int32_t Material::getVal() const {
	return this->average;
}

Material::Material(Loader& loader) {
	this->load(loader);
}

void Material::save(Saver& saver) {
	saver.store(this->elements.size());
	for (auto& e : elements) {
		e.save(saver);
	}
}

void Material::load(Loader& loader) {
	size_t size;
	loader.retrieve(size);
	for (size_t i = 0; i < size; i++) {
		this->elements.emplace_back(loader);
	}
}

Element::Element(ELEMENT::TYPE t, int32_t q) : type(t), quantity(q) {
}

Element::Element(Loader& loader) {
	this->load(loader);
}

void Element::save(Saver& saver) {
	saver.store(this->type);
	saver.store(quantity);
}

void Element::load(Loader& loader) {
	loader.retrieve(this->type);
}

BlockData const& ShapedBlock::getBlock() const {
	return this->block.getData();
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

ShapedBlock::ShapedBlock(std::string block, std::string shape, ACTIVITY::DIR dir) : block(block), shape(shape), rotation(dir) {
}

ShapedBlock::ShapedBlock(std::string block, SHAPE::TYPE shapeID, ACTIVITY::DIR rot) : block(block), shape(shapeID), rotation(rot) {
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

