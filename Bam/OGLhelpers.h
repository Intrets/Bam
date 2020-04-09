#pragma once

class ModelResource;

class ModelBinder
{
private:
	int vertex;
	int uv;
	int normal;

public:
	void bind(ModelResource model);

	ModelBinder(int vertex_, int uv_, int normal_);
	ModelBinder() = default;
	~ModelBinder() = default;
};

