#pragma once

#include "BufferWrappers.h"

constexpr auto MAX_CHARACTER_DRAW = 1 << 16;

struct TextRenderInfo;
struct CameraInfo;

// TODO: different fonts different renderers.
class TextRenderer
{
public:
	bwo::VertexArrayObject VAO;
	bwo::Program program;

	bwo::Buffer quad;
	bwo::Buffer characters;
	bwo::Buffer characterPositions;
	bwo::Buffer scale;

	glm::ivec2 fontDimension;
	glm::ivec2 gridDimension;
	float fontWidth;

	bwo::Uniform2iv fontDimensionUniform;
	bwo::Uniform2iv gridDimensionUniform;
	bwo::UniformTexture2D texture;

	bwo::Texture fontTexture;

	void render(TextRenderInfo& textRenderInfo, GLuint target, CameraInfo& camInfo);

	TextRenderer(std::string name, glm::ivec2 fontDim, glm::ivec2 gridDim);

	TextRenderer();
	~TextRenderer();
};

