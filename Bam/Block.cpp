#include "common.h"
#include "Block.h"
//#include "Activity.h"
#include "ActivityIgnoringGroup.h"
#include "Loader.h"
#include "Saver.h"

bool Block::isOccupied(ActivityIgnoringGroup& ignore) {
	if (this->m.isNotNull()) {
		return !ignore.contains(this->m.handle);
	}
	return this->isBlock();
}

bool Block::isOccupied() {
	return this->isActivity() || this->isBlock();
}

bool Block::isOccluded() {
	return this->occluded;
}

bool Block::isBlock() {
	return this->m.isNull() && this->blockID > 0;
}

bool Block::isActivity() {
	return this->m.isNotNull();
}

bool Block::load(Loader& loader) {
	loader.retrieve(this->blockID);
	loader.retrieve(this->occluded);
	loader.retrieve(this->m);
	return true;
}

bool Block::save(Saver& saver) {
	saver.store(this->blockID);
	saver.store(this->occluded);
	saver.store(this->m);
	return true;
}

Block::Block(int32_t id) {
	this->blockID = id;
}
