#include "common.h"

#include "BlockIDTextures.h"

#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "GLEnableWrapper.h"

#include "PassThroughRenderer.h"

int32_t BlockIDTextures::getBlockTextureID(std::string name) {
	auto search = this->textures.find(name);
	if (search == this->textures.end()) {
		// TODO : actually make texture loading work
		this->loadBlockTexture(name);
		this->textures[name] = this->arrayLayers;
		return this->arrayLayers++;
	}
	else {
		return search->second;
	}
}

void BlockIDTextures::loadBlockTexture(std::string name) {
	GLuint newTex = Locator<PathManager>::get()->LoadTextureP(name);
	glBindTexture(GL_TEXTURE_2D, newTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Failed to load texture
	if (newTex == 0) {
		printf("Failed to load texture: %s\n\n", name.c_str());
	}

	GLEnabler glEnabler;
	glEnabler.disable(GL_BLEND);

	Locator<PassThroughRenderer>::ref().render2DArray(this->textureArray.ID, this->arrayLayers, 0, 0, 0, 128, 128, newTex);
}

GLuint BlockIDTextures::getTextureArrayID() {
	return this->textureArray.ID;
}

BlockIDTextures::BlockIDTextures() {
	const std::string textures_raw[] = { "debug.dds", "debug.dds", "sandstone.dds", "dirt.dds",
		"stone.dds", "gravel.dds", "cobblestone.dds", "gravel2.dds", "mossy_cobblestone.dds", "diorite.dds", "andesite.dds", "podzol.dds", "fruit.dds" };

	glGenTextures(1, &this->textureArray.ID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->textureArray.ID);

	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &this->maxArrayLayers);
	this->maxArrayLayers = glm::max(1024, this->maxArrayLayers);
	// TODO: remove hardcoding of texture size
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 128, 128, this->maxArrayLayers);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for (auto& s : textures_raw) {
		this->loadBlockTexture(s);
		this->textures[s] = this->arrayLayers++;
	}
}
