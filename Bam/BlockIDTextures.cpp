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
	bwo::Texture2D newTex(Locator<PathManager>::get()->LoadTexture2DP(name));

	// Failed to load texture
	if (newTex.ID == 0) {
		printf("Failed to load texture: %s\n\n", name.c_str());
	}

	GLEnabler glEnabler;
	glEnabler.disable(GL_BLEND);

	Locator<PassThroughRenderer>::ref().render2DArray(
		this->textureArray,
		this->arrayLayers,
		0,
		newTex
	);
}

GLuint BlockIDTextures::getTextureArrayID() {
	return this->textureArray.ID;
}

BlockIDTextures::BlockIDTextures() :
	textureArray(bwo::Texture2DArrayHelper::makeLinearFiltering({ 128, 128, 1024 })) {
	const std::string textures_raw[] = { "debug.dds", "debug.dds", "sandstone.dds", "dirt.dds",
		   "stone.dds", "gravel.dds", "cobblestone.dds", "gravel2.dds", "mossy_cobblestone.dds", "diorite.dds", "andesite.dds", "podzol.dds", "fruit.dds" };

	for (auto& s : textures_raw) {
		this->loadBlockTexture(s);
		this->textures[s] = this->arrayLayers++;
	}
}
