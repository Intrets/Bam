#include "common.h"

#include "SelectionHighlightRenderer.h"
#include "GLEnableWrapper.h"
#include "RenderInfo.h"


SelectionHighlightRenderer::SelectionHighlightRenderer() :
	program(Locator<PathManager>::get()->LoadShadersP("HighlightShader.vert", "HighlightShader.frag")),
	VP("VP", this->program) {
	this->VAO.gen(4);

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
		0,                  // attribute
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*) 0            // array buffer offset
	);
	glVertexAttribDivisor(0, 0);

	this->offset.bind(GL_ARRAY_BUFFER);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(1, 1);

	this->scale.bind(GL_ARRAY_BUFFER);
	glVertexAttribPointer(
		2,                                // attribute
		2,                                // size
		GL_FLOAT,                           // type
		GL_FALSE,                         // normalized?
		0,							      // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(2, 1);

	this->color.bind(GL_ARRAY_BUFFER);
	glVertexAttribPointer(
		3,                                // attribute
		4,                                // size
		GL_FLOAT,                           // type
		GL_FALSE,                         // normalized?
		0,							      // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(3, 1);

	this->VAO.unbind();
}

SelectionHighlightRenderer::~SelectionHighlightRenderer() {
}

void SelectionHighlightRenderer::render(RenderInfo const& info, GLuint target) {
	this->VAO.bind();
	this->program.use();

	GLEnabler glEnabler;
	glEnabler.disable(GL_DEPTH_TEST).enable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, target);
	glViewport(0, 0, info.cameraInfo.x, info.cameraInfo.y);

	if (info.selectionRenderInfo.offsets.size() == 0) {
		return;
	}

	this->VP.set(info.cameraInfo.VP);

	int32_t drawCount = glm::min(MAX_STATIC_DRAW, static_cast<int32_t>(info.selectionRenderInfo.offsets.size()));

	this->offset.set(drawCount, info.selectionRenderInfo.offsets);
	this->scale.set(drawCount, info.selectionRenderInfo.scales);
	this->color.set(drawCount, info.selectionRenderInfo.colors);

	glDrawArraysInstanced(
		GL_TRIANGLES,
		0,
		6,
		static_cast<int32_t>(info.selectionRenderInfo.offsets.size())
	);

	this->VAO.unbind();
}
