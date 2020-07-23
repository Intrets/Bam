#pragma once

#include "BufferWrappers.h"

struct UIRenderInfo;
struct CameraInfo;

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

