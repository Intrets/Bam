#pragma once

#include "BufferWrappers.h"

#include <unordered_map>

class BlockIDTextures
{
private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;
	bwo::Buffer quad;
	bwo::Texture textureArray;
	bwo::Buffer textureBuffer;
	bwo::Uniform1i layer;
	bwo::UniformTexture2D texture;

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

