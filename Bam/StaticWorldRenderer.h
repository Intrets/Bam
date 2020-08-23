#pragma once

#include <array>

#include "BufferWrappers.h"

struct CameraInfo;
class StaticWorldChunk;
struct StaticWorldRenderInfo;

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

	void render(StaticWorldRenderInfo const& info, GLuint target, CameraInfo const& cameraInfo);
	void render(std::vector<StaticWorldRenderInfo*> const& infos, GLuint target, CameraInfo const& cameraInfo);
};

