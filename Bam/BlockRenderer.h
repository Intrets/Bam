#pragma once

#include "BufferWrappers.h"
#include <optional>

struct StaticWorldRenderInfo;
struct CameraInfo;
struct BlockRenderInfo;

class BlockRenderer
{
private:
	bwo::VertexArrayObject VAO;
	bwo::Program program;

	bwo::Buffer quad;
	bwo::Buffer blockInfos;

	bwo::Uniform1f depth;
	bwo::UniformMatrix4fv VP;
	bwo::UniformTexture2DArray texture;

public:
	int32_t const MAX_BATCH_SIZE = 5000;

	BlockRenderer();
	~BlockRenderer() = default;

	void render(std::vector<BlockRenderInfo> const& info, GLuint target, std::optional<float> depth_, CameraInfo const& cameraInfo);
};

