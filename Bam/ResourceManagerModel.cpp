#include "common.h"
#include "ResourceManagerModel.h"

#include <utility>

#include "ModelStore.h"

int32_t ResourceManagerModel::getIndex(std::string name) {
	int32_t index;
	if (indexMap.count(name) == 0) {
		index = static_cast<int32_t>(indexMap.size());
		indexMap[name] = index;
		names.push_back(name);
		resources.push_back(std::make_unique<ModelStore>(name));
	}
	else {
		index = indexMap[name];
	}
	return index;
}

ModelStore* ResourceManagerModel::get(int32_t index) {
	return resources[index].get();
}

ResourceManagerModel::ResourceManagerModel() {
}

ResourceManagerModel::~ResourceManagerModel() {
}

