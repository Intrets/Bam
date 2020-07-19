#include "common.h"

#include "Fonts.h"
#include "PassThroughRenderer.h"

Fonts::Fonts() {
	//TODO: config file?
	std::array<std::string, Font::FONT_MAX> fontNames = { "font_kaiser_90x199_22x6.dds" };

	glGenTextures(1, &fonts.ID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, fonts.ID);

	int32_t maxTextureLayers;
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTextureLayers);
	assert(maxTextureLayers >= Font::FONT_MAX);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipmapLayers, GL_RGBA8, texSize, texSize, Font::FONT_MAX);

	for (int32_t i = 0; i < Font::FONT_MAX; i++) {
		auto& fontName = fontNames[i];

		GLuint tex = Locator<PathManager>::ref().LoadFont(fontName);
		Locator<PassThroughRenderer>::ref().render2DArray(fonts.ID, i, 0, 0, 0, texSize, texSize, tex);
	}

	glGenerateTextureMipmap(fonts.ID);
}

Fonts::~Fonts() {
}
