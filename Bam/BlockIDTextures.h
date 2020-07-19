#pragma once

#include "BufferWrappers.h"

#include <unordered_map>

class BlockIDTextures
{
private:
	bwo::Texture textureArray;
	bwo::Buffer textureBuffer;

	int32_t maxArrayLayers;
	int32_t arrayLayers = 0;

	std::unordered_map<std::string, int32_t> textures;

public:
	int32_t getBlockTextureID(std::string);
	void loadBlockTexture(std::string);
	GLuint getTextureArrayID();

	NOCOPYMOVE(BlockIDTextures);

	BlockIDTextures();
	~BlockIDTextures() = default;
};

