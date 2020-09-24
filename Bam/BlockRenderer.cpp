#include "common.h"

#include "BlockRenderer.h"

BlockRenderer::BlockRenderer() :
	program(Locator<PathManager>::get()->LoadShadersP("BlockShader")),
	texture("textureSampler", this->program, 0),
	depth("depth", this->program),
	VP("VP", this->program)
{
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

	glGenBuffers(1, &this->rotation.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->rotation.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int32_t) * BATCH_DRAW_SIZE, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribIPointer(
		1,                                // attribute
		1,                                // size
		GL_INT,                           // type
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &this->textureID.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureID.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int32_t) * BATCH_DRAW_SIZE, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribIPointer(
		2,                                // attribute
		1,                                // size
		GL_INT,                           // type
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(2, 1);

	glGenBuffers(1, &this->stencilID.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->stencilID.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int32_t) * BATCH_DRAW_SIZE, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribIPointer(
		3,                                // attribute
		1,                                // size
		GL_INT,                           // type
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(2, 1);

	glGenBuffers(1, &this->position.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->position.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * BATCH_DRAW_SIZE, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(
		4,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(3, 1);
 
	this->VAO.unbind();
}

void BlockRenderer::render(StaticWorldRenderInfo const& info, GLuint terget, CameraInfo const& cameraInfo) {
}
