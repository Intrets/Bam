#pragma once

#include "BufferWrappers.h"

class PassThroughRenderer
{
public:
	bwo::Program program;

	bwo::VertexArrayObject VAO;

	bwo::Buffer quad;

	// 2D -> 2D array
	void render2DArray(GLuint arrayTextureTarget, int32_t layer, int32_t mipmap, int32_t x1, int32_t x2, int32_t y1, int32_t y2, GLuint textureSource);

	// 2D -> 2D
	void render2D(GLuint textureTarget, int32_t mipmap, int32_t x1, int32_t x2, int32_t y1, int32_t y2, GLuint textureSource);

	PassThroughRenderer();
	~PassThroughRenderer();
};

