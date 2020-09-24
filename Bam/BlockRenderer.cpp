#include "common.h"

#include "BlockRenderer.h"
#include "StaticWorldRenderInfo.h"
#include "GLEnableWrapper.h"
#include "BlockIDTextures.h"
#include "StaticWorldRenderInfo.h"
#include "CameraInfo.h"

BlockRenderer::BlockRenderer() :
	program(Locator<PathManager>::get()->LoadShadersP("BlockShader")),
	texture("textureSampler", this->program, 0),
	depth("depth", this->program),
	VP("VP", this->program) {
	this->VAO.gen(5);

	static const GLfloat g_quad_vertex_buffer_data[] = {
		0.0f,  0.0f,
		1.0f,  0.0f,
		0.0f,  1.0f,
		0.0f,  1.0f,
		1.0f,  0.0f,
		1.0f,  1.0f,
	};

	glGenBuffers(1, &this->quad.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->quad.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                   // attribute
		2,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		0,                   // stride
		(void*) 0            // array buffer offset
	);
	glVertexAttribDivisor(0, 0);

	glGenBuffers(1, &this->blockInfos.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->blockInfos.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BlockRenderInfo) * BATCH_DRAW_SIZE, NULL, GL_DYNAMIC_DRAW);


	constexpr int32_t stride = sizeof(BlockRenderInfo);
	size_t offset = 0;

	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		stride,							  // stride
		(void*) offset                    // array buffer offset
	);
	glVertexAttribDivisor(1, 1);

	offset += sizeof(decltype(BlockRenderInfo::pos));

	glVertexAttribIPointer(
		2,                                // attribute
		1,                                // size
		GL_INT,                           // type
		stride,							  // stride
		(void*) offset                    // array buffer offset
	);
	glVertexAttribDivisor(2, 1);

	offset += sizeof(decltype(BlockRenderInfo::rotation));

	glVertexAttribIPointer(
		3,                                // attribute
		1,                                // size
		GL_INT,                           // type
		stride,							  // stride
		(void*) offset                    // array buffer offset
	);
	glVertexAttribDivisor(3, 1);

	offset += sizeof(decltype(BlockRenderInfo::texture));

	glVertexAttribIPointer(
		4,                                // attribute
		1,                                // size
		GL_INT,                           // type
		stride,							  // stride
		(void*) offset                    // array buffer offset
	);
	glVertexAttribDivisor(4, 1);

	this->VAO.unbind();
}

void BlockRenderer::render(StaticWorldRenderInfo const& info, GLuint target, std::optional<float> depth_, CameraInfo const& cameraInfo) {
	this->VAO.bind();
	this->program.use();

	GLEnabler glEnabler;

	glEnabler.enable(GL_BLEND);

	if (depth_.has_value()) {
		glEnabler.enable(GL_DEPTH_TEST);
		this->depth.set(depth_.value());
	}
	else {
		glEnabler.disable(GL_DEPTH_TEST);
		this->depth.set(0.0f);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindFramebuffer(GL_FRAMEBUFFER, target);

	this->texture.set(Locator<BlockIDTextures>::ref().getTextureArrayID());
	this->VP.set(cameraInfo.VP);

	glm::ivec4 viewport{ 0, 0, cameraInfo.x, cameraInfo.y };
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	int32_t remaining = static_cast<int32_t>(info.blockRenderInfos.size());
	int32_t start = 0;

	glBindBuffer(GL_ARRAY_BUFFER, this->blockInfos.ID);

	while (remaining > 0) {
		int32_t size = glm::min(remaining, BATCH_DRAW_SIZE);

		constexpr int32_t byteSize = sizeof(BlockRenderInfo);

		glBufferSubData(GL_ARRAY_BUFFER, 0, byteSize * size, &info.blockRenderInfos[start]);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, size);

		start += size;
		remaining -= size;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	this->VAO.unbind();
}


