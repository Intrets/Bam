#include "common.h"
#include "BlockIDTextures.h"

#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "GLEnableWrapper.h"

#include "PassThroughRenderer.h"

int32_t BlockIDTextures::getBlockTextureID(std::string name) {
	auto search = textures.find(name);
	if (search == textures.end()) {
		// TODO : actually make texture loading work
		loadBlockTexture(name);
		textures[name] = arrayLayers;
		return arrayLayers++;
	}
	else {
		return search->second;
	}
}

void BlockIDTextures::loadBlockTexture(std::string name) {
	GLuint newTex = Locator<PathManager>::get()->LoadTextureP(name);
	// Failed to load texture
	if (newTex == 0) {
		printf("Failed to load texture: %s\n\n", name.c_str());
	}

	GLEnabler glEnabler;
	glEnabler.disable(GL_BLEND);

	Locator<PassThroughRenderer>::ref().render2DArray(textureArray.ID, arrayLayers, 0, 0, 0, 32, 32, newTex);
}

GLuint BlockIDTextures::getTextureArrayID() {
	return textureArray.ID;
}

BlockIDTextures::BlockIDTextures() {
	const std::string textures_raw[] = { "debug.dds", "debug.dds", "sandstone.dds", "dirt.dds",
		"stone.dds", "gravel.dds", "cobblestone.dds", "gravel2.dds", "mossy_cobblestone.dds", "diorite.dds", "andesite.dds", "podzol.dds", "fruit.dds" };

	glGenFramebuffers(1, &textureBuffer.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, textureBuffer.ID);

	glGenTextures(1, &textureArray.ID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray.ID);

	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxArrayLayers);
	printf("%d", maxArrayLayers);
	maxArrayLayers = glm::max(1024, maxArrayLayers);
	// TODO: remove hardcoding of texture size
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 32, 32, maxArrayLayers);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for (auto& s : textures_raw) {
		loadBlockTexture(s);
		textures[s] = arrayLayers++;
	}
}
