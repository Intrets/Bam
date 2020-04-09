#include "common.h"
#include "ResourceManagerModel.h"

#include <utility>

#include "ModelStore.h"

int ResourceManagerModel::getIndex(std::string name) {
	int index;
	if (indexMap.count(name) == 0) {
		index = indexMap.size();
		indexMap[name] = index;
		names.push_back(name);
		resources.push_back(std::make_unique<ModelStore>(name));
	}
	else {
		index = indexMap[name];
	}
	return index;
}

ModelStore* ResourceManagerModel::get(int index) {
	return resources[index].get();
}

ResourceManagerModel::ResourceManagerModel() {
}

ResourceManagerModel::~ResourceManagerModel() {
}

