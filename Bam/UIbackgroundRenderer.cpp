#include "common.h"

#include "UIbackgroundRenderer.h"

#include "ShaderLoader.h"
#include "GLEnableWrapper.h"
#include "UIRenderInfo.h"

void UIbackgroundRenderer::render(UIRenderInfo& renderInfo, GLuint target, CameraInfo& cameraInfo) {
	if (renderInfo.positions.empty()) {
		return;
	}
	VAO.bind();
	program.use();

	GLEnabler glEnabler;
	glEnabler.enable(GL_DEPTH_TEST).enable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, target);
	glViewport(0, 0, cameraInfo.x, cameraInfo.y);

	glBindBuffer(GL_ARRAY_BUFFER, positions.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * renderInfo.positions.size(), &renderInfo.positions[0]);

	glBindBuffer(GL_ARRAY_BUFFER, scales.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * renderInfo.scales.size(), &renderInfo.scales[0]);

	glBindBuffer(GL_ARRAY_BUFFER, colors.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * renderInfo.colors.size(), &renderInfo.colors[0]);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<int32_t>(renderInfo.scales.size()));
}

UIbackgroundRenderer::UIbackgroundRenderer() :
	program(Locator<PathManager>::get()->LoadShadersP("UIbg.vert", "UIbg.frag")) {
	static const GLfloat g_quad_vertex_buffer_data[] = {
		0.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
	};

	VAO.gen(4);

	glGenBuffers(1, &quad.ID);
	glBindBuffer(GL_ARRAY_BUFFER, quad.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*) 0            // array buffer offset
	);
	glVertexAttribDivisor(0, 0);

	glGenBuffers(1, &positions.ID);
	glBindBuffer(GL_ARRAY_BUFFER, positions.ID);
	// TODO: limit properly
	int32_t limit = 1000;
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * limit, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(
		1,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &scales.ID);
	glBindBuffer(GL_ARRAY_BUFFER, scales.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * limit, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(
		2,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(2, 1);

	glGenBuffers(1, &colors.ID);
	glBindBuffer(GL_ARRAY_BUFFER, colors.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * limit, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(
		3,                                // attribute
		4,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);

	glVertexAttribDivisor(3, 1);
}

UIbackgroundRenderer::~UIbackgroundRenderer() {
}
