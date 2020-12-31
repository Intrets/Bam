#include "common.h"

#include "WorldBlock.h"

#include "Activity.h"
#include "Saver.h"
#include "Loader.h"
#include "ActivityIgnoringGroup.h"

std::optional<WeakReference<Activity, Activity>> WorldBlock::getActivityMaybe() const {
	if (this->m != nullptr) {
		return this->m->getSelfReference();
	}
	else {
		return std::nullopt;
	}
}

WeakReference<Activity, Activity> WorldBlock::getActivity() const {
	return this->m->getSelfReference();
}

ACTIVITY::DIR WorldBlock::getRotation() const {
	return this->shapedBlock.rotation;
}

ShapedBlock const& WorldBlock::getShapedBlock() const {
	return this->shapedBlock;
}

void WorldBlock::setBlock(ShapedBlock block) {
	this->shapedBlock = block;
}

void WorldBlock::setTrace(Activity* h) {
	this->m = h;
}

void WorldBlock::removeTrace() {
	this->m = nullptr;
}

void WorldBlock::removeTrace(Activity* h) {
	if (this->m == h) {
		this->m = nullptr;
	}
}

bool WorldBlock::load(Loader& loader) {
	this->shapedBlock.load(loader);
	loader.retrieveActivityPointer(this->m);
	return true;
}

bool WorldBlock::save(Saver& saver) {
	this->shapedBlock.save(saver);
	saver.storeActivityPointer(this->m);
	return true;
}

WorldBlock::WorldBlock(std::string block, std::string shape) : shapedBlock(block, shape, ACTIVITY::DIR::RIGHT) {
}

WorldBlock::WorldBlock(int32_t id) : WorldBlock(id, 0) {
}

WorldBlock::WorldBlock(int32_t blockID, int32_t shapeID) : shapedBlock(blockID, shapeID, ACTIVITY::DIR::RIGHT) {
}

WorldBlock::WorldBlock(int32_t blockID, int32_t shapeID, ACTIVITY::DIR rotation) : shapedBlock(blockID, shapeID, rotation) {
}
bool WorldBlock::isOccupied(ActivityIgnoringGroup const& ignore) {
	if (this->m != nullptr) {
		return !ignore.contains(this->m);
	}
	return this->isSolid();
}

bool WorldBlock::isOccupied() {
	return this->isActivity() || this->isSolid();
}

bool WorldBlock::isSolid() {
	return this->shapedBlock.isSolid();
}

bool WorldBlock::isActivity() {
	return this->m != nullptr;
}

bool WorldBlock::isNonAirBlock() {
	return this->shapedBlock.isNonAir();
}

