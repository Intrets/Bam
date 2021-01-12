#include "common.h"

#include "PlayerActions.h"

#include "Loader.h"
#include "Saver.h"
#include "Network.h"

void PlayerActions::load(Loader& loader) {
	loader.retrieve(this->tick);

	int32_t size;
	loader.retrieve(size);

	this->operations.resize(size);
	for (size_t i = 0; i < size; i++) {
		this->operations[i] = OPERATION::loadOperation(loader);
	}
}

void PlayerActions::save(Saver& saver) {
	saver.store(this->tick);

	int32_t size = static_cast<int32_t>(this->operations.size());
	saver.store(size);

	for (auto& operation : this->operations) {
		operation->save(saver);
	}
}

void PlayerActions::append(Loader& loader) {
	int32_t size;
	loader.retrieve(size);

	int32_t start = static_cast<int32_t>(this->operations.size());
	int32_t end = start + size;

	this->operations.resize(end);
	for (size_t i = start; i < end; i++) {
		this->operations[i] = OPERATION::loadOperation(loader);
	}
}
