#include "common.h"

#include "MetaActions.h"

#include "MetaOperation.h"
#include "Loader.h"
#include "Saver.h"

void MetaActions::load(Loader& loader) {
	int32_t size;
	loader.retrieve(size);

	this->operations.resize(size);
	for (size_t i = 0; i < size; i++) {
		this->operations[i] = METAOPERATION::loadOperation(loader);
	}
}

void MetaActions::save(Saver& saver) {
	int32_t size = static_cast<int32_t>(this->operations.size());
	saver.store(size);

	for (auto& operation : this->operations) {
		operation->save(saver);
	}
}
