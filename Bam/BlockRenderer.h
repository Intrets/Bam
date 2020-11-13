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

	bwo::ArrayBuffer<glm::vec2> quad{ bwo::BufferHint::STATIC_DRAW };
	bwo::ArrayBuffer<BlockRenderInfo> blockInfos{ bwo::BufferHint::STREAM_DRAW };

	bwo::Uniform1f depth;
	bwo::UniformMatrix4fv VP;
	bwo::UniformTexture2DArray texture;

public:
	int32_t const MAX_BATCH_SIZE = 5000;

	BlockRenderer();
	~BlockRenderer() = default;

	void render(std::vector<BlockRenderInfo> const& info, bwo::FrameBuffer& target, std::optional<float> depth_, CameraInfo const& cameraInfo);
};

