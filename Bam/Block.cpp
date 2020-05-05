#include "common.h"
#include "Block.h"
//#include "Activity.h"
//#include "ActivityIgnoringGroup.h"
#include "Loader.h"
#include "Saver.h"

bool Block::isOccupied(ActivityIgnoringGroup& ignore) {
	if (blockID == 1) {
		//return m && !ignore.contains(m.handle);
	}
	else if (blockID > 1) {
		return true;
	}
	return false;
}

bool Block::isOccupied() {
	return blockID;
}

bool Block::isOccluded() {
	return occluded;
}

bool Block::isBlock() {
	return blockID > 1;
}

bool Block::isActivity() {
	return blockID == 1;
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
