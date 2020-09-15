#pragma once

#include "BufferWrappers.h"
#include <optional>

class BlitRendererArrayTexture
{
private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;

	bwo::Buffer quad;
	bwo::Buffer rotation;
	bwo::Buffer layer;
	bwo::Buffer position;

	bwo::UniformTexture2DArray texture_t;
	bwo::Uniform1f depth;
	bwo::UniformMatrix4fv VP;

public:
	void render(std::vector<glm::vec2> const& positions, std::vector<int32_t> const& rotations, std::vector<int32_t> const& layers, GLuint target, glm::ivec4 viewport, GLuint texture, std::optional<float> depth_, glm::mat4 VP);

	BlitRendererArrayTexture();
	~BlitRendererArrayTexture() = default;
};

