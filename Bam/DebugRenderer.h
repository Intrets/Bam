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
	bwo::Buffer data;

	void renderVector(std::vector<glm::vec2> const& line, int32_t type, RenderInfo const& renderInfo);

public:
	void render(GLuint target, RenderInfo const& renderInfo);

	DebugRenderer();
	~DebugRenderer();
};

