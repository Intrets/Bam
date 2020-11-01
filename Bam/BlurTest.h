#pragma once

#include "BufferWrappers.h"

class BlurTest
{
private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;

	bwo::ArrayBuffer<glm::vec2> quad{ bwo::BufferHint::STATIC_DRAW };
	bwo::UniformTexture2D textureUniform;

public:


};

