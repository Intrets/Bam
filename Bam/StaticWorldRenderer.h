#pragma once

#include <array>

#include "BufferWrappers.h"
#include "StaticWorldRenderInfo.h"

struct CameraInfo;
class StaticWorldChunk;

class StaticWorldRenderer
{
private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;
	bwo::Buffer offset;
	bwo::Buffer textureID;

	bwo::UniformMatrix4fv VP;
	bwo::UniformTexture2DArray texture;

public:
	StaticWorldRenderer();
	~StaticWorldRenderer();

	void render(StaticWorldRenderInfo& info, GLuint target, CameraInfo& cameraInfo);
	void render(std::vector<StaticWorldRenderInfo*> infos, GLuint target, CameraInfo& cameraInfo);
};

