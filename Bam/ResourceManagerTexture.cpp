#include "common.h"

#include "ResourceManagerTexture.h"

int32_t ResourceManagerTexture::getIndex(std::string name) {
	int32_t index;
	if (this->indexMap.count(name) == 0) {
		index = static_cast<int32_t>(this->indexMap.size());
		this->indexMap[name] = index;
		this->names.push_back(name);
		this->resources.push_back(std::make_unique<TextureStore>(name));
	}
	else {
		index = this->indexMap[name];
	}
	return index;
}

TextureStore* ResourceManagerTexture::get(int32_t index) {
	return this->resources[index].get();
}

ResourceManagerTexture::ResourceManagerTexture() {
}

ResourceManagerTexture::~ResourceManagerTexture() {
}
