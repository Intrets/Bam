#include "common.h"
#include "SelectionHighlightRenderer.h"
#include "ModelResource.h"
#include "ModelStore.h"
#include "GLEnableWrapper.h"


SelectionHighlightRenderer::SelectionHighlightRenderer() :
	program(Locator<PathManager>::getService()->LoadShadersP("HighlightShader.vert", "HighlightShader.frag")),
	VP("VP", program) {
	VAO.gen(4);

	ModelResource tempp("devtile.obj");

	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, tempp.get()->vertexbufferHandle);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*) 0            // array buffer offset
	);

	glGenBuffers(1, &offset.ID);
	glBindBuffer(GL_ARRAY_BUFFER, offset.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * MAX_STATIC_DRAW, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &scale.ID);
	glBindBuffer(GL_ARRAY_BUFFER, scale.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * MAX_STATIC_DRAW, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(
		2,                                // attribute
		2,                                // size
		GL_FLOAT,                           // type
		GL_FALSE,                         // normalized?
		0,							      // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(2, 1);

	glGenBuffers(1, &color.ID);
	glBindBuffer(GL_ARRAY_BUFFER, color.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * MAX_STATIC_DRAW, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(
		3,                                // attribute
		4,                                // size
		GL_FLOAT,                           // type
		GL_FALSE,                         // normalized?
		0,							      // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(3, 1);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempp.get()->indexbufferHandle);

	VAO.unbind();
}


SelectionHighlightRenderer::~SelectionHighlightRenderer() {
}

void SelectionHighlightRenderer::render(RenderInfo & info, GLuint target) {
	ModelResource tempp("devtile.obj");

	VAO.bind();
	program.use();

	GLEnabler glEnabler;
	glEnabler.disable(GL_DEPTH_TEST).enable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, target);
	glViewport(0, 0, info.cameraInfo.x, info.cameraInfo.y);

	if (info.selectionRenderInfo.offsets.size() == 0) {
		return;
	}

	VP.set(info.cameraInfo.VP);

	int drawCount = glm::min(MAX_STATIC_DRAW, static_cast<int>(info.selectionRenderInfo.offsets.size()));

	glBindBuffer(GL_ARRAY_BUFFER, offset.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(decltype(info.selectionRenderInfo.offsets)::value_type) * drawCount, &info.selectionRenderInfo.offsets[0]);

	glBindBuffer(GL_ARRAY_BUFFER, scale.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(decltype(info.selectionRenderInfo.scales)::value_type) * drawCount, &info.selectionRenderInfo.scales[0]);

	glBindBuffer(GL_ARRAY_BUFFER, color.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * drawCount, &info.selectionRenderInfo.colors[0]);

	glDrawElementsInstanced(
		GL_TRIANGLES,      // mode
		tempp.get()->indexbufferSize,    // count
		GL_UNSIGNED_SHORT, // type
		(void*) 0,           // element array buffer offset
		drawCount
	);

	VAO.unbind();
}