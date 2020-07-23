#include "common.h"

#include "ModelStore.h"

ModelStore::ModelStore(std::string name) {
	Locator<PathManager>::get()->LoadModelP(name, this->vertexbufferHandle, this->uvbufferHandle, this->normalbufferHandle, this->indexbufferHandle, this->indexbufferSize);
}

ModelStore::~ModelStore() {
	glDeleteBuffers(1, &this->vertexbufferHandle);
	glDeleteBuffers(1, &this->uvbufferHandle);
	glDeleteBuffers(1, &this->normalbufferHandle);
	glDeleteBuffers(1, &this->indexbufferHandle);
	this->indexbufferSize = 0;
}
