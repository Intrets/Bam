#include "common.h"
#include "ResourceManagerModel.h"

#include <utility>

#include "ModelStore.h"

int32_t ResourceManagerModel::getIndex(std::string name) {
	int32_t index;
	if (this->indexMap.count(name) == 0) {
		index = static_cast<int32_t>(this->indexMap.size());
		this->indexMap[name] = index;
		this->names.push_back(name);
		this->resources.push_back(std::make_unique<ModelStore>(name));
	}
	else {
		index = this->indexMap[name];
	}
	return index;
}

ModelStore* ResourceManagerModel::get(int32_t index) {
	return this->resources[index].get();
}

ResourceManagerModel::ResourceManagerModel() {
}

ResourceManagerModel::~ResourceManagerModel() {
}

