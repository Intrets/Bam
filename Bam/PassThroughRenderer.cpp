#include "common.h"

#include "PassThroughRenderer.h"

void PassThroughRenderer::render2DArray(GLuint arrayTextureTarget, int32_t layer, int32_t mipmap, int32_t x1, int32_t y1, int32_t x2, int32_t y2, GLuint textureSource) {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.ID);
	program.use();
	VAO.bind();
	texture.set(textureSource);
	glViewport(x1, y1, x2, y2);
	glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, arrayTextureTarget, mipmap, layer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	VAO.unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PassThroughRenderer::render2D(GLuint textureTarget, int32_t mipmap, int32_t x1, int32_t y1, int32_t x2, int32_t y2, GLuint textureSource) {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.ID);
	program.use();
	VAO.bind();
	texture.set(textureSource);
	glViewport(x1, y1, x2, y2);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureTarget, mipmap);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	VAO.unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PassThroughRenderer::PassThroughRenderer() :
	program(Locator<PathManager>::get()->LoadShadersP("Passthrough")),
	texture("texture_t", program, 0) {

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	glGenFramebuffers(1, &frameBuffer.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.ID);

	VAO.gen(1);

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

	VAO.unbind();
}

PassThroughRenderer::~PassThroughRenderer() {
}
