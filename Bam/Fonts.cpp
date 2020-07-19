#include "common.h"

#include "Fonts.h"

Fonts::Fonts() {
	glGenTextures(1, &fonts.ID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, fonts.ID);

	int32_t maxTextureLayers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTextureLayers);
	assert(maxTextureLayers <= Font::FONT_MAX);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipmapLayers, GL_RGBA8, texSize, texSize, Font::FONT_MAX);

	//Locator < PathManager>::ref().openFile()
}

Fonts::~Fonts() {
}
