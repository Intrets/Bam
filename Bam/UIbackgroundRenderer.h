#pragma once

#include "BufferWrappers.h"
#include "CameraInfo.h"

struct UIRenderInfo;

class UIbackgroundRenderer
{
public:
	bwo::VertexArrayObject VAO;
	bwo::Program program;

	bwo::Buffer quad;
	bwo::Buffer positions;
	bwo::Buffer scales;
	bwo::Buffer colors;

	void render(UIRenderInfo& renderInfo, GLuint target, CameraInfo& camInfo);

	UIbackgroundRenderer();
	~UIbackgroundRenderer();
};

