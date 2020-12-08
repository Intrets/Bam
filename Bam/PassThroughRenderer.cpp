#include "common.h"

#include "PassThroughRenderer.h"

void PassThroughRenderer::render2DArray(bwo::Texture2DArray& target, int32_t layer, int32_t mipmap, bwo::Texture2D& source) {
	program.use();
	VAO.bind();
	texture.set(source);

	this->frameBuffer.bindTextureLayer(GL_COLOR_ATTACHMENT0, target, mipmap, layer);
	this->frameBuffer.draw(
		{ target.size.x, target.size.y },
		{ 0, 0, target.size.x, target.size.y },
		[&]()
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
	});

	VAO.unbind();
}

PassThroughRenderer::PassThroughRenderer() :
	program("Passthrough", "PassThroughRenderer"),
	texture("texture_t", program, 0) {
	static const GLfloat g_quad_vertex_buffer_data[] = {
		   -1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
		   -1.0f,  1.0f, 0.0f,
		   -1.0f,  1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
	};

	VAO.gen(1);

	this->quad.setRaw(sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data);
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
