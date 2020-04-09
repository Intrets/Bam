#include "common.h"
#include "ModelStore.h"


ModelStore::ModelStore(std::string name) {
	Locator<PathManager>::getService()->LoadModelP(name, vertexbufferHandle, uvbufferHandle, normalbufferHandle, indexbufferHandle, indexbufferSize);
}

ModelStore::~ModelStore() {
	glDeleteBuffers(1, &vertexbufferHandle);
	glDeleteBuffers(1, &uvbufferHandle);
	glDeleteBuffers(1, &normalbufferHandle);
	glDeleteBuffers(1, &indexbufferHandle);
	indexbufferSize = 0;
}
