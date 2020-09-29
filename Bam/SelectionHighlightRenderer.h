#pragma once

#include "BufferWrappers.h"

struct RenderInfo;

class SelectionHighlightRenderer
{
private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;
	bwo::Buffer quad;
	bwo::Buffer offset;
	bwo::Buffer scale;
	bwo::Buffer color;

	bwo::UniformMatrix4fv VP;

public:
	SelectionHighlightRenderer();
	~SelectionHighlightRenderer();

	void render(RenderInfo const& info, GLuint target);
};

