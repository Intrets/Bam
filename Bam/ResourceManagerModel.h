#pragma once

#include <unordered_map>

class ModelStore;

class ResourceManagerModel
{
private:
	//TODO: add error texture and model?
	std::unordered_map<std::string, int> indexMap;
	std::vector<std::unique_ptr<ModelStore>> resources;
	std::vector<std::string> names;

public:
	int getIndex(std::string name);
	ModelStore* get(int index);

	ResourceManagerModel();
	~ResourceManagerModel();
};