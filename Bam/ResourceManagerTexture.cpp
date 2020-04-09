#include "common.h"

#include "ResourceManagerTexture.h"

int ResourceManagerTexture::getIndex(std::string name) {
	int index;
	if (indexMap.count(name) == 0) {
		index = indexMap.size();
		indexMap[name] = index;
		names.push_back(name);
		resources.push_back(std::make_unique<TextureStore>(name));
	}
	else {
		index = indexMap[name];
	}
	return index;
}

TextureStore* ResourceManagerTexture::get(int index) {
	return resources[index].get();
}

ResourceManagerTexture::ResourceManagerTexture() {
}

ResourceManagerTexture::~ResourceManagerTexture() {
}
