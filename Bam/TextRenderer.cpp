#include "common.h"

#include "TextRenderer.h"

#include "CameraInfo.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "GLEnableWrapper.h"
#include "CameraInfo.h"
#include "TextRenderInfo.h"

void TextRenderer::render(TextRenderInfo& textRenderInfo, GLuint target, CameraInfo& cameraInfo) {
	if (textRenderInfo.offsets.size() == 0) {
		return;
	}

	VAO.bind();
	program.use();

	fontDimensionUniform.set(fontDimension);
	gridDimensionUniform.set(gridDimension);
	texture.set(fontTexture.ID);

	GLEnabler glEnabler;
	glEnabler.enable(GL_BLEND).disable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, target);
	glViewport(0, 0, cameraInfo.x, cameraInfo.y);

	glBindBuffer(GL_ARRAY_BUFFER, characters.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int32_t) * textRenderInfo.chars.size(), &textRenderInfo.chars[0]);

	glBindBuffer(GL_ARRAY_BUFFER, characterPositions.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * textRenderInfo.offsets.size(), &textRenderInfo.offsets[0]);

	glBindBuffer(GL_ARRAY_BUFFER, scale.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * textRenderInfo.scales.size(), &textRenderInfo.scales[0]);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<int32_t>(textRenderInfo.chars.size()));

	VAO.unbind();
}

TextRenderer::TextRenderer(std::string name, glm::ivec2 fontDim, glm::ivec2 gridDim) :
	fontDimension(fontDim),
	gridDimension(gridDim),
	fontWidth(fontDim.y / static_cast<float>(fontDim.x)),
	program(Locator<PathManager>::getService()->LoadShadersP("Text.vert", "Text.frag")),
	fontDimensionUniform("fontDim", program),
	gridDimensionUniform("texDim", program),
	texture("texture", program, 0) {
	fontTexture.ID = Locator<PathManager>::getService()->LoadTextureP(name);

	glBindTexture(GL_TEXTURE_2D, fontTexture.ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

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
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*) 0            // array buffer offset
	);
	glVertexAttribDivisor(0, 0);


	glGenBuffers(1, &characters.ID);
	glBindBuffer(GL_ARRAY_BUFFER, characters.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int32_t) * MAX_CHARACTER_DRAW, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribIPointer(
		1,                                // attribute
		1,                                // size
		GL_INT,							  // type
		0,							      // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &characterPositions.ID);
	glBindBuffer(GL_ARRAY_BUFFER, characterPositions.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_CHARACTER_DRAW, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(2, 1);

	glGenBuffers(1, &scale.ID);
	glBindBuffer(GL_ARRAY_BUFFER, scale.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * MAX_CHARACTER_DRAW, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(
		3,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(3, 1);

	VAO.unbind();
}

TextRenderer::TextRenderer() {
}

TextRenderer::~TextRenderer() {
}
