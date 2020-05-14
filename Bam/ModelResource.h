#pragma once

class ModelStore;

class ModelResource
{
private:
	int32_t index;

public:
	ModelStore* get();

	ModelResource(std::string name);
	ModelResource() = default;
	~ModelResource() = default;
};