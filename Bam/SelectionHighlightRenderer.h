#pragma once

#include "BufferWrappers.h"

struct RenderInfo;

class SelectionHighlightRenderer
{
private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;
	bwo::ArrayBuffer<glm::vec4> quad{ bwo::BufferHint::STATIC_DRAW };
	bwo::ArrayBuffer<glm::vec2> offset{ bwo::BufferHint::STREAM_DRAW };
	bwo::ArrayBuffer<glm::vec2> scale{ bwo::BufferHint::STREAM_DRAW };
	bwo::ArrayBuffer<glm::vec4> color{ bwo::BufferHint::STREAM_DRAW };

	bwo::UniformMatrix4fv VP;

public:
	SelectionHighlightRenderer();
	~SelectionHighlightRenderer();

	void render(RenderInfo const& info, GLuint target);
};

