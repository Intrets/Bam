#include "common.h"

#include "BlitRendererArrayTexture.h"
#include "GLEnableWrapper.h"

void BlitRendererArrayTexture::render(std::vector<glm::vec2> const& positions, std::vector<int32_t> const& rotations, std::vector<int32_t> const& layers, GLuint target, glm::ivec4 viewport, GLuint texture, std::optional<float> depth_, glm::mat4 VP_) {
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

	this->texture_t.set(texture);
	this->VP.set(VP_);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	int32_t remaining = static_cast<int32_t>(positions.size());
	int32_t start = 0;

	while (remaining > 0) {
		int32_t size = glm::min(remaining, BATCH_DRAW_SIZE);

		glBindBuffer(GL_ARRAY_BUFFER, this->position.ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * size, &positions[start]);

		glBindBuffer(GL_ARRAY_BUFFER, this->rotation.ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int32_t) * size, &rotations[start]);

		glBindBuffer(GL_ARRAY_BUFFER, this->layer.ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int32_t) * size, &layers[start]);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, size);

		start += size;
		remaining -= size;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	this->VAO.unbind();
}

BlitRendererArrayTexture::BlitRendererArrayTexture() :
	program(Locator<PathManager>::get()->LoadShadersP("BlitArray")),
	texture_t("texture_t", this->program, 0),
	depth("depth", this->program),
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

	glGenBuffers(1, &this->layer.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->layer.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int32_t) * BATCH_DRAW_SIZE, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribIPointer(
		2,                                // attribute
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
		3,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(3, 1);

	this->VAO.unbind();
}
