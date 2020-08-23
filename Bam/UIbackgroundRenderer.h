#pragma once

#include "BufferWrappers.h"

struct UIRenderInfo;
struct CameraInfo;

class UIbackgroundRenderer
{
private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;

	bwo::Buffer quad;
	bwo::Buffer positions;
	bwo::Buffer scales;
	bwo::Buffer colors;

public:
	void render(UIRenderInfo const& renderInfo, GLuint target, CameraInfo const& camInfo);

	UIbackgroundRenderer();
	~UIbackgroundRenderer();
};

