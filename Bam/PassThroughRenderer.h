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
	void render2DArray(bwo::Texture2DArray& target, int32_t layer, int32_t mipmap, bwo::Texture2D& source);

	PassThroughRenderer();
	~PassThroughRenderer();
};

