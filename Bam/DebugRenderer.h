#pragma once

#include <vector>

#include "BufferWrappers.h"

struct CameraInfo;
struct RenderInfo;
class DebugRenderInfo;

class DebugRenderer
{
public:
	void render(GLuint target, RenderInfo& renderInfo);
	//void render(GLuint target, CameraInfo& cameraInfo);

	DebugRenderer();
	~DebugRenderer();

private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;
	bwo::UniformMatrix4fv VP;
	bwo::Buffer data;

	void renderVector(std::vector<glm::vec2>& line, int type, RenderInfo& renderInfo);
};

