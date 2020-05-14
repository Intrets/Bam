#pragma once

class ModelResource;

class ModelBinder
{
private:
	int32_t vertex;
	int32_t uv;
	int32_t normal;

public:
	void bind(ModelResource model);

	ModelBinder(int32_t vertex_, int32_t uv_, int32_t normal_);
	ModelBinder() = default;
	~ModelBinder() = default;
};

