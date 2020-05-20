#pragma once

class ModelStore
{
public:
	GLuint vertexbufferHandle;
	GLuint uvbufferHandle;
	GLuint normalbufferHandle;
	GLuint indexbufferHandle;
	int32_t indexbufferSize;

	ModelStore(std::string name);
	ModelStore() = delete;
	~ModelStore();

	NOCOPYMOVE(ModelStore);
};
