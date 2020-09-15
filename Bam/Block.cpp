#include "common.h"
#include "Block.h"
#include "ActivityIgnoringGroup.h"
#include "Loader.h"
#include "Saver.h"
#include <fstream>
#include "StringHelpers.h"
#include "BlockIDTextures.h"


std::array<BlockData, MAXBLOCKS> Block::data{};
std::unordered_map<std::string, int32_t> Block::nameMap{};
int32_t Block::blockCount = 0;

BlockData* Block::operator->() {
	return &Block::data[this->blockID];
}

ELEMENT::TYPE ELEMENT::getType(std::string s) {
	return ELEMENT::typeMap[s];
}
std::string ELEMENT::getName(ELEMENT::TYPE type) {
	switch (type) {
		case ELEMENT::AIR:
			return "air";
			break;
		case ELEMENT::SILICONDIOXIDE:
			return "silicondioxide";
			break;
		case ELEMENT::IRON:
			return "iron";
			break;
		case ELEMENT::IRONOXIDE:
			return "ironoxide";
			break;
		case ELEMENT::CARBON:
			return "carbon";
			break;
		case ELEMENT::HYDROCARBON:
			return "hydrocarbon";
			break;
		default:
			return "";
			break;
	}
}

void Block::loadBlocks() {
	std::ifstream file;
	assert(Locator<PathManager>::ref().openBlockData(file));

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

		Block::data[i].name = pairs["name"];
		Block::data[i].solid = pairs["solid"] == "true" ? true : false;
		Block::data[i].texture = Locator<BlockIDTextures>::ref().getBlockTextureID(pairs["texture"]);

		std::vector<std::string> elementPairs;

		split(0, pairs["elements"], elementPairs, ',');

		for (auto& elementPair : elementPairs) {
			std::vector<std::string> e;
			split(0, elementPair, e, ':');
			Block::data[i].elements.push_back({ ELEMENT::getType(e[0]), std::stoi(e[1].c_str()) });
		}

		Block::nameMap[pairs["name"]] = i;

		Block::blockCount++;
		i++;
	}
}

int32_t Block::getBlockCount() {
	return Block::blockCount;
}

bool Block::isOccupied(ActivityIgnoringGroup const& ignore) {
	if (this->m.isNotNull()) {
		return !ignore.contains(this->m.handle);
	}
	return this->isSolid();
}

bool Block::isOccupied() {
	return this->isActivity() || this->isSolid();
}

bool Block::isSolid() {
	return this->solid;
}

bool Block::isActivity() {
	return this->m.isNotNull();
}

bool Block::isNonAirBlock() {
	return this->blockID != 0;
}

void Block::setBlockID(int32_t id) {
	this->blockID = id;
	this->solid = (*this)->solid;
}

void Block::setBlockID(int32_t id, ACTIVITY::DIR rotation_) {
	this->setBlockID(id);
	this->rotation = rotation_;
}

int32_t Block::getBlockID() {
	return this->blockID;
}

int32_t Block::getTexture() {
	return (*this)->texture;
}

std::string const& Block::getBlockName() {
	return (*this)->name;
}

int32_t Block::getBlockID(std::string name) {
	auto it = Block::nameMap.find(name);
	if (it != Block::nameMap.end()) {
		return Block::nameMap[name];
	}
	return 0;
}

std::optional<WeakReference<Activity, Activity>> Block::getActivityMaybe() const {
	if (this->m.isNotNull()) {
		return this->m;
	}
	else {
		return std::nullopt;
	}
}

WeakReference<Activity, Activity> Block::getActivity() const {
	return this->m;
}

ACTIVITY::DIR Block::getRotation() const {
	return this->rotation;
}

void Block::setTrace(Handle h) {
	this->m.handle = h;
}

void Block::removeTrace() {
	this->m.handle = 0;
}

void Block::removeTrace(Handle h) {
	if (this->m.handle == h) {
		this->m.handle = 0;
	}
}

Block::Block(Block&& other) {
	this->blockID = other.blockID;
	this->solid = other.solid;
	this->m = other.m;

	other.blockID = 0;
	other.solid = false;
	other.m.handle = 0;
}

Block& Block::operator=(Block&& other) {
	if (this == &other) {
		return *this;
	}
	else {
		this->blockID = other.blockID;
		this->solid = other.solid;
		this->m = other.m;

		other.blockID = 0;
		other.solid = false;
		other.m.handle = 0;
		return *this;
	}
}

bool Block::load(Loader& loader) {
	loader.retrieve(this->blockID);
	loader.retrieve(this->m);
	loader.retrieve(this->solid);
	return true;
}

bool Block::save(Saver& saver) {
	saver.store(this->blockID);
	saver.store(this->m);
	saver.store(this->solid);
	return true;
}

Block::Block(int32_t id) : blockID(id) {
	this->solid = (*this)->solid;
}

Block::Block(int32_t id, ACTIVITY::DIR rotation_) : Block(id) {
	this->rotation = rotation_;
}

