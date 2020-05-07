#pragma once

#include "BufferWrappers.h"
#include "RenderInfo.h"

class SelectionHighlightRenderer
{
public:
	bwo::VertexArrayObject VAO;
	bwo::Program program;
	bwo::Buffer offset;
	bwo::Buffer scale;
	bwo::Buffer color;

	bwo::UniformMatrix4fv VP;

	SelectionHighlightRenderer();
	~SelectionHighlightRenderer();
	
	void render(RenderInfo& info, GLuint target);
};

