#pragma once

#include "BufferWrappers.h"

struct CameraInfo;
struct RenderInfo;
class DebugRenderInfo;

class DebugRenderer
{
private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;
	bwo::UniformMatrix4fv VP;
	bwo::ArrayBuffer<glm::vec2> data{ bwo::BufferHint::STREAM_DRAW };

	void renderVector(std::vector<glm::vec2> const& line, int32_t type, RenderInfo const& renderInfo);

public:
	void render(GLuint target, RenderInfo const& renderInfo);

	DebugRenderer();
	~DebugRenderer();
};

