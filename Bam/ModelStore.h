#pragma once

class ModelStore
{
public:
	GLuint vertexbufferHandle;
	GLuint uvbufferHandle;
	GLuint normalbufferHandle;
	GLuint indexbufferHandle;
	int indexbufferSize;

	ModelStore(std::string name);
	ModelStore() = delete;
	~ModelStore();

	deleteDefaults(ModelStore);
};
