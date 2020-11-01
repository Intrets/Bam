#pragma once

#include "BufferWrappers.h"

// TODO: rework
class PassThroughRenderer
{
private:
	bwo::Program program;

	bwo::VertexArrayObject VAO;
	bwo::UniformTexture2D texture;

	bwo::ArrayBuffer<glm::vec3> quad{ bwo::BufferHint::STATIC_DRAW };
	bwo::FrameBuffer frameBuffer;

public:
	// 2D -> 2D array
	void render2DArray(GLuint arrayTextureTarget, int32_t layer, int32_t mipmap, int32_t x1, int32_t x2, int32_t y1, int32_t y2, GLuint textureSource);

	// 2D -> 2D
	void render2D(GLuint textureTarget, int32_t mipmap, int32_t x1, int32_t x2, int32_t y1, int32_t y2, GLuint textureSource);

	PassThroughRenderer();
	~PassThroughRenderer();
};

