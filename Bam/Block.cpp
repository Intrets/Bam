#include "common.h"
#include "Block.h"
//#include "Activity.h"
#include "ActivityIgnoringGroup.h"
#include "Loader.h"
#include "Saver.h"

bool Block::isOccupied(ActivityIgnoringGroup& ignore) {
	if (m.isNotNull()) {
		return !ignore.contains(m.handle);
	}
	return isBlock();
}

bool Block::isOccupied() {
	return isActivity() || isBlock();
}

bool Block::isOccluded() {
	return occluded;
}

bool Block::isBlock() {
	return m.isNull() && blockID > 0;
}

bool Block::isActivity() {
	return m.isNotNull();
}

bool Block::load(Loader& loader) {
	loader.retrieve(blockID);
	loader.retrieve(occluded);
	loader.retrieve(m);
	return true;
}

bool Block::save(Saver& saver) {
	saver.store(blockID);
	saver.store(occluded);
	saver.store(m);
	return true;
}

Block::Block(int id) {
	blockID = id;
}
