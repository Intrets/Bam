#pragma once
#include <unordered_map>
#include <memory>
#include "TextureStore.h"

class TextureStore;

class ResourceManagerTexture
{
private:
	//TODO: add error texture and model?
	std::unordered_map<std::string, int> indexMap;
	std::vector<std::unique_ptr<TextureStore>> resources;
	std::vector<std::string> names;

public:
	int getIndex(std::string name);
	TextureStore* get(int index);

	ResourceManagerTexture();
	~ResourceManagerTexture();
};

