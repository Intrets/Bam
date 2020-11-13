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


	this->quad.setRaw(sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data);
	glVertexAttribPointer(
		0,                   // attribute
		2,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		0,                   // stride
		(void*) 0            // array buffer offset
	);
	glVertexAttribDivisor(0, 0);

	this->blockInfos.bind(GL_ARRAY_BUFFER);

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

void BlockRenderer::render(std::vector<BlockRenderInfo> const& info, bwo::FrameBuffer& target, std::optional<float> depth_, CameraInfo const& cameraInfo) {
	if (info.size() == 0) {
		return;
	}

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

	this->texture.set(Locator<BlockIDTextures>::ref().getTextureArrayID());
	this->VP.set(cameraInfo.VP);
	this->blockInfos.set(info);

	target.draw(
		{ cameraInfo.x, cameraInfo.y },
		{ 0,0,cameraInfo.x, cameraInfo.y },
		[&]()
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(info.size()));
	});

	this->VAO.unbind();
}
