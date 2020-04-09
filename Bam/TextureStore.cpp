#include "common.h"
#include "TextureStore.h"

TextureStore::TextureStore(std::string name) {
	textureHandle = Locator<PathManager>::getService()->LoadTextureP(name);
}

TextureStore::~TextureStore() {
	glDeleteTextures(1, &textureHandle);
}
