#pragma once

#include <unordered_map>
#include <memory>

#include "TextureStore.h"

class ResourceManagerTexture
{
private:
	//TODO: add error texture and model?
	std::unordered_map<std::string, int32_t> indexMap;
	std::vector<std::unique_ptr<TextureStore>> resources;
	std::vector<std::string> names;

public:
	int32_t getIndex(std::string name);
	TextureStore* get(int32_t index);

	ResourceManagerTexture();
	~ResourceManagerTexture();
};

