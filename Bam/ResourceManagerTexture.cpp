#include "common.h"

#include "ResourceManagerTexture.h"

int32_t ResourceManagerTexture::getIndex(std::string name) {
	int32_t index;
	if (indexMap.count(name) == 0) {
		index = static_cast<int32_t>(indexMap.size());
		indexMap[name] = index;
		names.push_back(name);
		resources.push_back(std::make_unique<TextureStore>(name));
	}
	else {
		index = indexMap[name];
	}
	return index;
}

TextureStore* ResourceManagerTexture::get(int32_t index) {
	return resources[index].get();
}

ResourceManagerTexture::ResourceManagerTexture() {
}

ResourceManagerTexture::~ResourceManagerTexture() {
}
