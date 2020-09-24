#pragma once

#include "BufferWrappers.h"

class StaticWorldRenderInfo;
struct CameraInfo;

class BlockRenderer
{
private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;

	bwo::Buffer quad;
	bwo::Buffer rotation;
	bwo::Buffer position;
	bwo::Buffer textureID;
	bwo::Buffer stencilID;

	bwo::Uniform1f depth;
	bwo::UniformMatrix4fv VP;
	bwo::UniformTexture2DArray texture;

public:
	int32_t const MAX_BATCH_SIZE = 5000;

	BlockRenderer();
	~BlockRenderer() = default;

	void render(StaticWorldRenderInfo const& info, GLuint terget, CameraInfo const& cameraInfo);
};

