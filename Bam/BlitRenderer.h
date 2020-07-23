#pragma once

#include "BufferWrappers.h"
#include <optional>

class BlitRenderer
{
private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;

	bwo::Buffer quad;
	bwo::Buffer UVSource;
	bwo::Buffer worldTarget;

	bwo::UniformTexture2D texture_t;
	bwo::Uniform1f UVflip;
	bwo::Uniform1f depth;

public:
	int32_t const MAX_BATCH_SIZE = 1000;

	void render(std::vector<glm::vec4>& uv, std::vector<glm::vec4>& world, GLuint target, glm::ivec4 viewport, GLuint texture, std::optional<float> depth_, bool flipUVvertical);

	void render(glm::vec4 uv, glm::vec4 world, GLuint target, glm::ivec4 viewport, GLuint texture, std::optional<float> depth_, bool fromTexture);

	BlitRenderer();
	~BlitRenderer();
};

