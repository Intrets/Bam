#pragma once

#include <unordered_map>

class ModelStore;

class ResourceManagerModel
{
private:
	//TODO: add error texture and model?
	std::unordered_map<std::string, int32_t> indexMap;
	std::vector<std::unique_ptr<ModelStore>> resources;
	std::vector<std::string> names;

public:
	int32_t getIndex(std::string name);
	ModelStore* get(int32_t index);

	ResourceManagerModel();
	~ResourceManagerModel();
};