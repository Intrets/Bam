#include "common.h"

#include "BlitRenderer.h"
#include "GLEnableWrapper.h"

void BlitRenderer::render(std::vector<glm::vec4>& uv, std::vector<glm::vec4>& world, GLuint target, glm::ivec4 viewport, GLuint texture, std::optional<float> depth_, bool flipUVvertical, glm::vec2 offset_, std::optional<glm::vec4> maybeColor) {
	this->VAO.bind();
	this->program.use();

	GLEnabler glEnabler;
	glEnabler.enable(GL_BLEND);

	this->offset.set(offset_);

	if (maybeColor.has_value()) {
		this->color.set(maybeColor.value());
	}
	else {
		this->color.set(glm::vec4(1.0f));
	}

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

	if (flipUVvertical) {
		this->UVflip.set(-1.0f);
	}
	else {
		this->UVflip.set(1.0f);
	}

	this->texture_t.set(texture);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	//glViewport(0, 0, 1024/2, 1024/2);

	int32_t remaining = static_cast<int32_t>(uv.size());
	int32_t start = 0;

	while (remaining > 0) {
		int32_t size = glm::min(remaining, MAX_BATCH_SIZE);

		glBindBuffer(GL_ARRAY_BUFFER, this->UVSource.ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * size, &uv[start]);

		glBindBuffer(GL_ARRAY_BUFFER, this->worldTarget.ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * size, &world[start]);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, size);

		start += size;
		remaining -= size;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	this->VAO.unbind();
}

void BlitRenderer::render(glm::vec4 uv, glm::vec4 world, GLuint target, glm::ivec4 viewport, GLuint texture, std::optional<float> depth_, bool flipUVvertical, std::optional<glm::vec4> maybeColor) {
	std::vector uvv{ uv };
	std::vector worldv{ world };
	this->render(uvv, worldv, target, viewport, texture, depth_, flipUVvertical, glm::vec2(0.0f), maybeColor);
}

BlitRenderer::BlitRenderer() :
	program(Locator<PathManager>::ref().LoadShadersP("Blit")),
	UVflip("UVflip", this->program),
	texture_t("texture_t", this->program, 0),
	depth("depth", this->program),
	offset("offset", this->program),
	color("c", this->program) {
	this->VAO.gen(3);

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
		0,                  // attribute
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*) 0            // array buffer offset
	);
	glVertexAttribDivisor(0, 0);

	glGenBuffers(1, &this->UVSource.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->UVSource.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * MAX_BATCH_SIZE, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(
		1,                                // attribute
		4,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &this->worldTarget.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->worldTarget.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * MAX_BATCH_SIZE, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(
		2,                                // attribute
		4,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(2, 1);

	this->VAO.unbind();
}

BlitRenderer::~BlitRenderer() {
}
