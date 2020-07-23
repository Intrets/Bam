#include "common.h"
#include "TextureStore.h"

TextureStore::TextureStore(std::string name) {
	this->textureHandle = Locator<PathManager>::get()->LoadTextureP(name);
}

TextureStore::~TextureStore() {
	glDeleteTextures(1, &this->textureHandle);
}
